#include "json_reader.h"

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

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
    return Document { Node { document.GetRoot().AsMap().at("render_settings"s).AsMap() } };
}

void Reader::BaseRequestHandle() {
    const Array base_requests(document.GetRoot().AsMap().at("base_requests"s).AsArray());

    StopBaseRequestHandle(base_requests);
    BusBaseRequestHandle(base_requests);
}

void Reader::StopBaseRequestHandle(const Array& base_requests) {
    for (const auto& request : base_requests) {
        const Dict& dictionary = request.AsMap();
        if (dictionary.at("type"s).AsString() == "Stop"s) {
            catalogue
                .AddStop(dictionary.at("name"s).AsString(),
                    dictionary.at("latitude"s).AsDouble(),
                    dictionary.at("longitude"s).AsDouble());
            for (const auto& [stop, distance] : dictionary.at("road_distances"s).AsMap()) {
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
        const Dict& dictionary = request.AsMap();
        if (dictionary.at("type"s).AsString() == "Bus"s) {
            std::vector<const domain::Stop*> stops;
            const Array arr_stops(dictionary.at("stops"s).AsArray());
            for (const auto& stop : arr_stops) {
                stops.push_back(catalogue.FindStop(stop.AsString()));
            }

            if (!dictionary.at("is_roundtrip"s).AsBool()) {
                stops.reserve(2 * stops.size());
                for (int64_t i = stops.size() - 2; i > -1; --i) {
                    stops.push_back(stops[i]);
                }
            }

            double geo_length = .0;
            int64_t length = 0;
            AgreeDistances(stops, length, geo_length);
            catalogue.AddBus(dictionary.at("name"s).AsString(), stops, length, geo_length);
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
    const Array stat_requests(document.GetRoot().AsMap().at("stat_requests"s).AsArray());

    for (const auto& request : stat_requests) {
        const auto& type = request.AsMap().at("type"s).AsString();
        const int id = request.AsMap().at("id"s).AsInt();
        if (type == "Stop"s) {
            StopStatRequestHandle(request, id);
        } else if (type == "Bus"s) {
            BusStatRequestHandle(request, id);
        }
    }
}

void Reader::BusStatRequestHandle(const Node& request, int id) {
    auto [check, size, unique_size, distance, geo_distance] = catalogue.GetBusInfo(request.AsMap().at("name"s).AsString());
            
    Dict dictionary;
    dictionary.insert({ "request_id"s, Node { id } });           
            
    if (!check) {
        dictionary.insert({ "error_message"s, Node { "not found"s } });
        stat_response.push_back(Node { dictionary });
        return;
    }
  
    dictionary.insert({ "curvature"s, Node { static_cast<double>(distance / geo_distance) } });
    dictionary.insert({ "route_length"s, Node { static_cast<int>(distance) } });
    dictionary.insert({ "stop_count"s, Node { static_cast<int>(size) } });
    dictionary.insert({ "unique_stop_count"s, Node { static_cast<int>(unique_size) } });

    stat_response.push_back(Node { dictionary });
}

void Reader::StopStatRequestHandle(const Node& request, int id) {
    Dict dictionary;
    dictionary.insert({ "request_id"s, Node { id } }); 

    const auto& stop = request.AsMap().at("name").AsString();
    if (!catalogue.CheckStop(stop)) {
        dictionary.insert({ "error_message"s, Node { "not found"s } });
        stat_response.push_back(Node { dictionary });
        return;
    }

    const std::set<std::string_view>& buses = catalogue.GetBusesInStop(stop);
    Array result;
    for (const auto& bus : buses) {
        result.push_back(Node { static_cast<std::string>(bus) });
    }
    dictionary.insert({ "buses"s, Node { result } });

    stat_response.push_back(Node { dictionary });
}

const TransportCatalogue& Reader::GetCatalogue() const {
    return catalogue;
}

} // namespace JsonReader