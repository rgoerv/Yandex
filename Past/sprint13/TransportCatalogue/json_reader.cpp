#include "json.h"
#include "json_reader.h"
#include "json_builder.h"
#include "map_renderer.h"
#include "transport_catalogue.h"
#include "transport_router.h"

#include <sstream>

namespace JsonReader {

Reader::Reader(std::istream& input)
    : document(json::Load(input))
{
    BaseRequestHandle();
    StatRequestHandle();
}

void Reader::Reply(std::ostream& output) const {
    Print(Document { stat_response }, output);
}

const json::Document Reader::GetRenderSettings() const {
    return Document { Node { document.GetRoot().AsDict().at("render_settings"s).AsDict() } };
}

void Reader::BaseRequestHandle() {
    const Array base_requests(document.GetRoot().AsDict().at("base_requests"s).AsArray());

    StopBaseRequestHandle(base_requests);
    BusBaseRequestHandle(base_requests);
}

void Reader::StopBaseRequestHandle(const Array& base_requests) {
    for (const auto& request : base_requests) {
        const Dict& dictionary = request.AsDict();
        if (dictionary.at("type"s).AsString() == "Stop"s) {
            catalogue
                .AddStop(dictionary.at("name"s).AsString(),
                    dictionary.at("latitude"s).AsDouble(),
                    dictionary.at("longitude"s).AsDouble());
            for (const auto& [stop, distance] : dictionary.at("road_distances"s).AsDict()) {
                stops_to_dstns.insert({{ dictionary.at("name"s).AsString(), stop }, distance.AsInt()});
            }
        }
    }

    for (const auto& [stops, distance] : stops_to_dstns) {
        const auto& [from, to] = stops;
        catalogue.AddDistance(from, to, distance);
    }
}

void Reader::BusBaseRequestHandle(const Array& base_reauest) {
    for (const auto& request : base_reauest) {
        const Dict& dictionary = request.AsDict();
        if (dictionary.at("type"s).AsString() == "Bus"s) {
            std::vector<const domain::Stop*> stops;
            const Array arr_stops(dictionary.at("stops"s).AsArray());
            for (const auto& stop : arr_stops) {
                stops.push_back(catalogue.FindStop(stop.AsString()));
            }

            bool is_roundtrip = dictionary.at("is_roundtrip"s).AsBool();
            const Stop* last_stop = stops.back();
            if (is_roundtrip == false) {
                stops.reserve(2 * stops.size());
                for (int64_t i = stops.size() - 2; i > -1; --i) {
                    stops.push_back(stops[i]);
                }
                is_roundtrip = true;
            }

            double geo_length = .0;
            int64_t length = 0;
            AgreeDistances(stops, length, geo_length);
            catalogue.AddBus(dictionary.at("name"s).AsString(), stops, length, geo_length, is_roundtrip, last_stop);
        }
    }
}

void Reader::AgreeDistances(const std::vector<const domain::Stop*>& stops, int64_t& length, double& geo_length) const {
    const Stop* first = stops.front();
    const Stop* last;

    for (const auto& stop : stops) {
        last = stop;
        geo_length += geo::ComputeDistance(first->coordinates, last->coordinates);
        const int64_t distance =  catalogue.GetDistance(first, last);
        length += distance > 0 ? distance : catalogue.GetDistance(last, first);     
        first = last;
    }
}

void Reader::StatRequestHandle() {
    const Array stat_requests(document.GetRoot().AsDict().at("stat_requests"s).AsArray());

    static graph graph_(catalogue.GetStopCount() * 2);
    bool need_build_router = true;

    for (const auto& request : stat_requests) {
        const auto& type = request.AsDict().at("type"s).AsString();
        if (type == "Stop"s) {
            StopStatRequestHandle(request);
        } else if (type == "Bus"s) {
            BusStatRequestHandle(request);
        } else if (type == "Map") {
            MapStatRequestHandle(request);
        } else if (type == "Route") {
            if (need_build_router) {
                BuildGraph(graph_); // build one graph with all, to use in transport router
                BuildTransportRouter(graph_);
                need_build_router = false;
            }
            RouterStatRequestHandle(request);
        }
    }
}

void Reader::BuildGraph(graph& graph_) {

    constexpr const static double M = 1'000;
    constexpr const static double MIN = 60;
    constexpr const static double CONVERSION = M / MIN;

    const RoutingSettings routing_settings(GetRoutingSettings());
    VertexId general_id = 0;
    
    for (const auto& bus : catalogue.GetBuses()) {
        const auto& stops = bus.route_;

        size_t start = 0;
        for (auto stop = stops.begin(); stop + 1 != stops.end(); ++stop) {

            VertexId from = general_id;
            if (stop_to_vertex.count(*stop) > 0) {
                from = stop_to_vertex[*stop];
            } else {
                stop_to_vertex[*stop] = from;
                id_to_stop[from] = *stop;
                ++general_id;
            }

            size_t count = 1; // count stops between start and next bus stop - span count
            for (auto following_stop = stop + 1; following_stop != stops.end(); ++following_stop) {         

                VertexId to = general_id;
                if (stop_to_vertex.count(*following_stop) > 0) {
                    to = stop_to_vertex[*following_stop];
                }
                else {
                    stop_to_vertex[*following_stop] = to;
                    id_to_stop[to] = *following_stop;
                    ++general_id;
                }

                graph_.AddEdge({ from, to, routing_settings.bus_wait_time_  +
                     ((catalogue.GetDistance(&bus, start, count) * 1.0) / (routing_settings.bus_velocity_ * CONVERSION)) });

                edge_to_bus_span[{from, to}] = {&bus, count};
                ++count;
            }
            ++start;
        }
    }
}

void Reader::BuildTransportRouter(graph& graph_) {
    router = std::move(std::make_unique<TRouter::TransportRouter>(TRouter::TransportRouter{
        std::move(std::make_unique<graph>(graph_)), 
        std::move(std::make_unique<Router<double>>(Router<double>{graph_})),
        std::move(std::make_unique<Stop_VertexId>(stop_to_vertex)),
        std::move(std::make_unique<Edge_BusSpan>(edge_to_bus_span)),
        std::move(std::make_unique<VertexId_Sport>(id_to_stop)),
        GetCatalogue(), GetRoutingSettings() }));
}

void Reader::BusStatRequestHandle(const Node& request) {
    auto [check, size, unique_size, distance, geo_distance] = catalogue.GetBusInfo(request.AsDict().at("name"s).AsString());
    const int id = request.AsDict().at("id"s).AsInt();
    
    json::Builder builder;
    builder.StartDict().Key("request_id").Value(id);       
            
    if (!check) {
        builder.Key("error_message"s).Value("not found"s).EndDict();
        stat_response.push_back(builder.Build());
        return;
    }

    builder
        .Key("curvature"s)
        .Value(static_cast<double>(distance / geo_distance))
        .Key("route_length"s)
        .Value(static_cast<int>(distance))
        .Key("stop_count"s)
        .Value(static_cast<int>(size))
        .Key("unique_stop_count"s)
        .Value(static_cast<int>(unique_size))
        .EndDict();
    stat_response.push_back(builder.Build());
}

void Reader::StopStatRequestHandle(const Node& request) {
    const int id = request.AsDict().at("id"s).AsInt();

    json::Builder builder;
    builder.StartDict().Key("request_id"s).Value(id);

    const auto& stop = request.AsDict().at("name").AsString();
    if (!catalogue.CheckStop(stop)) {
        builder.Key("error_message"s).Value("not found"s).EndDict();
        stat_response.push_back(builder.Build());
        return;
    }

    const std::set<std::string_view>& buses = catalogue.GetBusesInStop(stop);
    json::Builder arr_builder;
    arr_builder.StartArray();
    for (const auto& bus : buses) {
        arr_builder.Value(static_cast<std::string>(bus));
    }
    arr_builder.EndArray();

    builder.Key("buses"s).Value(arr_builder.Build().AsArray()).EndDict();

    stat_response.push_back(builder.Build());
}

void Reader::MapStatRequestHandle(const Node& request) {
    renderer::MapRenderer renderer(GetRenderSettings(), GetCatalogue());

    std::stringstream render_out;
    renderer.Render(render_out);

    json::Builder builder;
    builder.StartDict()
        .Key("map"s)
        .Value(render_out.str())
        .Key("request_id"s)
        .Value(request.AsDict().at("id"s).AsInt())
        .EndDict();

    stat_response.push_back(builder.Build());
}

void Reader::RouterStatRequestHandle(const Node& request) {

    json::Builder builder;
    builder.StartDict().Key("request_id"s).Value(request.AsDict().at("id"s).AsInt());

    std::string_view from = request.AsDict().at("from"s).AsString();
    std::string_view to = request.AsDict().at("to"s).AsString();

    if (catalogue.GetBusesInStop(from).size() == 0 || catalogue.GetBusesInStop(to).size() == 0) {
        builder.Key("error_message"s).Value("not found"s).EndDict();
        stat_response.push_back(builder.Build());
        return;
    }

    const auto& route_info = (*router).GetRouteInfo(from, to);
    
    if (!route_info) {
        builder.Key("error_message"s).Value("not found"s).EndDict();
        stat_response.push_back(builder.Build());
        return;
    }
    
    builder.Key("total_time"s).Value(json::Node((*route_info).total_time).AsDouble());
    builder.Key("items"s).StartArray();

    for (const auto& item : (*route_info).items) {
        builder.StartDict();
        switch (item.type)
        {
            case RouteReqestType::WAIT: builder.Key("type"s).Value("Wait"s)
                .Key("stop_name"s).Value(json::Node(static_cast<std::string>(*(item.stop_name))).AsString())
                .Key("time"s).Value(json::Node(item.time).AsDouble());
                break;
            case RouteReqestType::BUS: builder.Key("type"s).Value("Bus"s)
                .Key("bus"s).Value(json::Node(static_cast<std::string>(*(item.bus_name))).AsString())
                .Key("time"s).Value(json::Node(item.time).AsDouble())
                .Key("span_count"s).Value(json::Node(static_cast<int>(*(item.span_count))).AsInt());
                break;
            default:
                break;
        }
        builder.EndDict();
    }
    builder.EndArray().EndDict();
    stat_response.push_back(builder.Build());
}

const TransportCatalogue& Reader::GetCatalogue() const {
    return catalogue;
}

const domain::RoutingSettings Reader::GetRoutingSettings() const {
    const Dict routing_settings = document.GetRoot().AsDict().at("routing_settings"s).AsDict();
    return domain::RoutingSettings{ routing_settings.at("bus_wait_time"s).AsDouble(),
                                    routing_settings.at("bus_velocity"s).AsDouble() };
}

} // namespace JsonReader