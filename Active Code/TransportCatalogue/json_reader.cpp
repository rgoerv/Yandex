#include "json.h"
#include "json_reader.h"
#include "json_builder.h"
#include "map_renderer.h"

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

    for (const auto& request : stat_requests) {
        const auto& type = request.AsDict().at("type"s).AsString();
        if (type == "Stop"s) {
            StopStatRequestHandle(request);
        } else if (type == "Bus"s) {
            BusStatRequestHandle(request);
        } else if (type == "Map") {
            MapStatRequestHandle(request);
        }
    }
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

const TransportCatalogue& Reader::GetCatalogue() const {
    return catalogue;
}

} // namespace JsonReader