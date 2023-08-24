#include "json_reader.h"

#include "json_builder.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "transport_catalogue.h"
#include "transport_router.h"

#include <algorithm>
#include <chrono>
#include <sstream>
#include <vector>

namespace transport_catalogue::json_reader {

using namespace std::literals;

namespace {

TransportCatalogueBuilder::BusDesc ReadBusDesc(const json::Dict& bus_json) {
    TransportCatalogueBuilder::BusDesc req;
    req.name = bus_json.at("name"s).AsString();
    req.is_roundtrip = bus_json.at("is_roundtrip"s).AsBool();

    const auto& stops_json = bus_json.at("stops"s).AsArray();
    req.stop_names.reserve(stops_json.size());
    for (const auto& stop_json : stops_json) {
        req.stop_names.emplace_back(stop_json.AsString());
    }
    return req;
}

TransportCatalogueBuilder::StopDesc ReadStopDesc(const json::Dict& stop_json) {
    TransportCatalogueBuilder::StopDesc req;
    req.name = stop_json.at("name"s).AsString();
    req.latitude = stop_json.at("latitude"s).AsDouble();
    req.longitude = stop_json.at("longitude"s).AsDouble();

    if (const auto distances_it = stop_json.find("road_distances"s);
        distances_it != stop_json.end()) {
        const auto& neighbors_json = stop_json.at("road_distances"s).AsDict();
        req.road_distances.reserve(neighbors_json.size());
        for (const auto& [name, distance] : neighbors_json) {
            req.road_distances.emplace_back(name, distance.AsInt());
        }
    }

    return req;
}

// -------------

uint8_t ReadByte(const json::Node& json) {
    int int_value = json.AsInt();
    uint8_t byte = static_cast<uint8_t>(int_value);
    if (static_cast<int>(byte) == int_value) {
        return byte;
    }
    throw std::out_of_range(std::to_string(int_value) + " is out of byte range"s);
}

svg::Color ReadColor(const json::Node& json) {
    if (json.IsArray()) {
        const auto& arr = json.AsArray();
        if (arr.size() == 3) {  // Rgb
            return svg::Rgb(ReadByte(arr[0]), ReadByte(arr[1]), ReadByte(arr[2]));
        } else if (arr.size() == 4) {  // Rgba
            return svg::Rgba(ReadByte(arr[0]), ReadByte(arr[1]), ReadByte(arr[2]),
                             arr[3].AsDouble());
        }
    } else if (json.IsString()) {
        return json.AsString();
    } else if (json.IsNull()) {
        return svg::NoneColor;
    }
    throw InvalidRequestError("invalid color format");
}

svg::Point ReadPoint(const json::Array& json) {
    if (json.size() != 2) {
        throw std::invalid_argument("Point array must have exactly 2 elements");
    }
    return svg::Point{json[0].AsDouble(), json[1].AsDouble()};
}

std::vector<svg::Color> ReadColors(const json::Array& json) {
    std::vector<svg::Color> colors;
    colors.reserve(json.size());

    for (const auto& item : json) {
        colors.emplace_back(ReadColor(item));
    }

    return colors;
}

// -------------

const json::Dict NOT_FOUND_RESPONSE_JSON =
    json::Builder{}.StartDict()
        .Key("error_message"s).Value("not found"s)
    .EndDict().Build().AsDict();

struct BusRequest {
    std::string name;

    json::Dict Execute(const service::RequestHandler& handler) const {
        if (const auto bus_stat = handler.GetBusStat(name)) {
            return json::Builder{}.StartDict()
                .Key("stop_count"s).Value(static_cast<int>(bus_stat->total_stops))
                .Key("unique_stop_count"s).Value(static_cast<int>(bus_stat->unique_stops))
                .Key("route_length"s).Value(bus_stat->route_length)
                .Key("curvature"s).Value(bus_stat->route_length / bus_stat->route_length_direct)
            .EndDict().Build().AsDict();
        } else {
            return NOT_FOUND_RESPONSE_JSON;
        }
    }
};

struct StopRequest {
    std::string name;

    json::Dict Execute(const service::RequestHandler& handler) const {
        if (const auto buses = handler.GetBusesByStop(name)) {
            std::vector<BusPtr> bus_vector{buses->begin(), buses->end()};
            std::sort(bus_vector.begin(), bus_vector.end(), [](BusPtr lhs, BusPtr rhs) {
                return lhs->name < rhs->name;
            });

            json::Array bus_names;
            bus_names.reserve(buses->size());
            for (const auto& bus : bus_vector) {
                bus_names.emplace_back(bus->name);
            }

            return json::Builder{}.StartDict()
                .Key("buses"s).Value(std::move(bus_names))
            .EndDict().Build().AsDict();
        } else {
            return NOT_FOUND_RESPONSE_JSON;
        }
    }
};

struct MapRequest {
    json::Dict Execute(const service::RequestHandler& handler) const {
        std::ostringstream strm;
        handler.RenderMap().Render(strm);
        return json::Builder{}.StartDict()
            .Key("map"s).Value(strm.str())
        .EndDict().Build().AsDict();
    }
};

json::Node ConvertRouteItemToJson(const router::RouteInfo::BusItem& item) {
    return json::Builder{}.StartDict()
        .Key("type"s).Value("Bus"s)
        .Key("bus"s).Value(item.bus_ptr->name)
        .Key("time"s).Value(item.time.count())
        .Key("span_count"s).Value(static_cast<int>(item.span_count))
    .EndDict().Build();
}

json::Node ConvertRouteItemToJson(const router::RouteInfo::WaitItem& item) {
    return json::Builder{}.StartDict()
        .Key("type"s).Value("Wait"s)
        .Key("stop_name"s).Value(item.stop_ptr->name)
        .Key("time"s).Value(item.time.count())
    .EndDict().Build();
}

struct RouteRequest {
    std::string name_from;
    std::string name_to;

    json::Dict Execute(const service::RequestHandler& handler) const {
        if (const auto route = handler.FindRoute(name_from, name_to)) {
            json::Array items;
            items.reserve(route->items.size());
            for (const auto& item : route->items) {
                std::visit(
                    [&items](const auto& specific_item) {
                        items.push_back(ConvertRouteItemToJson(specific_item));
                    },
                    item);
            }

            return json::Builder{}.StartDict()
                .Key("total_time"s).Value(route->total_time.count())
                .Key("items"s).Value(std::move(items))
            .EndDict().Build().AsDict();
        } else {
            return NOT_FOUND_RESPONSE_JSON;
        }
    }
};

using StatRequest = std::variant<BusRequest, StopRequest, MapRequest, RouteRequest>;

StatRequest ReadStatRequest(const json::Dict& attrs) {
    const std::string& type = attrs.at("type"s).AsString();
    if (type == "Bus"sv) {
        return BusRequest{attrs.at("name"s).AsString()};
    } else if (type == "Stop"sv) {
        return StopRequest{attrs.at("name"s).AsString()};
    } else if (type == "Map"sv) {
        return MapRequest{};
    } else if (type == "Route"sv) {
        return RouteRequest{
            attrs.at("from"s).AsString(), 
            attrs.at("to"s).AsString()
        };
    } else {
        throw InvalidRequestError("Invalid request type "s + type);
    }
}

json::Dict HandleRequest(const json::Dict& request_json, const service::RequestHandler& handler) {
    const auto request = ReadStatRequest(request_json);
    auto response_json = std::visit(
        [&handler](const auto& request) {
            return request.Execute(handler);
        },
        request);
    response_json["request_id"s] = request_json.at("id"s).AsInt();
    return response_json;
}

}  // namespace

TransportCatalogue ReadTransportCatalogue(const json::Array& base_requests_json) {
    TransportCatalogueBuilder builder;

    for (const auto& req_json : base_requests_json) {
        const auto& props_json = req_json.AsDict();
        const std::string& type = props_json.at("type"s).AsString();
        if (type == "Bus"sv) {
            builder.AddBus(ReadBusDesc(props_json));
        } else if (type == "Stop"sv) {
            builder.AddStop(ReadStopDesc(props_json));
        } else {
            throw InvalidRequestError("Invalid type");
        }
    }

    return builder.Build();
}

renderer::RenderSettings ReadRenderSettings(const json::Dict& render_settings_json) {
    renderer::RenderSettings rs;

    rs.palette = ReadColors(render_settings_json.at("color_palette"s).AsArray());

    rs.underlayer_width = render_settings_json.at("underlayer_width"s).AsDouble();
    rs.underlayer_color = ReadColor(render_settings_json.at("underlayer_color"s));

    rs.max_width = render_settings_json.at("width"s).AsDouble();
    rs.max_height = render_settings_json.at("height"s).AsDouble();
    rs.padding = render_settings_json.at("padding"s).AsDouble();

    rs.stop_radius = render_settings_json.at("stop_radius"s).AsDouble();
    rs.line_width = render_settings_json.at("line_width"s).AsDouble();

    rs.stop_label_offset = ReadPoint(render_settings_json.at("stop_label_offset"s).AsArray());
    rs.stop_label_font_size = render_settings_json.at("stop_label_font_size"s).AsInt();

    rs.bus_label_font_size = render_settings_json.at("bus_label_font_size").AsInt();
    rs.bus_label_offset = ReadPoint(render_settings_json.at("bus_label_offset").AsArray());

    return rs;
}

router::RoutingSettings ReadRoutingSettings(const json::Dict& routing_settings_json) {
    router::RoutingSettings rs;
    rs.bus_wait_time = std::chrono::minutes(routing_settings_json.at("bus_wait_time"s).AsInt());
    rs.bus_velocity_kmh = routing_settings_json.at("bus_velocity"s).AsDouble();
    return rs;
}

json::Array HandleRequests(const json::Array& requests_json,
                           const service::RequestHandler& handler) {
    json::Array responses_json;
    responses_json.reserve(requests_json.size());

    for (const json::Node& request_json : requests_json) {
        responses_json.emplace_back(HandleRequest(request_json.AsDict(), handler));
    }

    return responses_json;
}

}  // namespace transport_catalogue::json_reader