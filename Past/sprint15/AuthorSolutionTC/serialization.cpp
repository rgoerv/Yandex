#include "serialization.h"

#include "graph.h"
#include "map_renderer.h"
#include "router.h"
#include "svg.h"
#include "transport_catalogue.h"
#include "transport_router.h"

#include <graph.pb.h>
#include <map_renderer.pb.h>
#include <svg.pb.h>

#include <cassert>
#include <unordered_map>
#include <utility>
#include <variant>


namespace transport_catalogue::serialization {

namespace {

svg_proto::Point SerializePoint(svg::Point point) {
    svg_proto::Point proto;
    proto.set_x(point.x);
    proto.set_y(point.y);
    return proto;
}

svg::Point DeserializePoint(const svg_proto::Point& proto) {
    return {proto.x(), proto.y()};
}

svg_proto::Color SerializeColor(std::monostate) {
    svg_proto::Color proto;
    proto.set_is_none(true);
    return proto;
}

svg_proto::Color SerializeColor(const std::string& name) {
    svg_proto::Color proto;
    proto.set_name(name);
    return proto;
}

svg_proto::Color SerializeColor(const svg::Rgb& rgb) {
    svg_proto::Color proto;
    auto& rgba_proto = *proto.mutable_rgba();
    rgba_proto.set_red(rgb.red);
    rgba_proto.set_green(rgb.green);
    rgba_proto.set_blue(rgb.blue);
    return proto;
}

svg_proto::Color SerializeColor(const svg::Rgba& rgba) {
    auto proto = SerializeColor(svg::Rgb{rgba.red, rgba.green, rgba.blue});
    auto& rgba_proto = *proto.mutable_rgba();
    rgba_proto.set_has_opacity(true);
    rgba_proto.set_opacity(rgba.opacity);
    return proto;
}

svg_proto::Color SerializeColor(const svg::Color& color) {
    return std::visit(
        [](const auto& color) { return SerializeColor(color); },
        color
    );
}

svg::Color DeserializeColor(const svg_proto::Color& proto) {
    if (proto.is_none()) {
        return svg::NoneColor;
    }

    if (!proto.has_rgba()) {
        return proto.name();
    }

    const auto& rgba_proto = proto.rgba();
    const auto red = static_cast<uint8_t>(rgba_proto.red());
    const auto green = static_cast<uint8_t>(rgba_proto.green());
    const auto blue = static_cast<uint8_t>(rgba_proto.blue());
    
    if (rgba_proto.has_opacity()) {
        return svg::Rgba{red, green, blue, rgba_proto.opacity()};
    } else {
        return svg::Rgb{red, green, blue};
    }
}


using StopIndices = std::unordered_map<StopPtr, uint32_t>;

StopIndices BuildStopIndices(const TransportCatalogue& db) {
    StopIndices stop_indices;
    uint32_t index = 0;
    for (const Stop& stop : db.GetStops()) {
        stop_indices[&stop] = index++;
    }
    return stop_indices;
}

using BusIndices = std::unordered_map<BusPtr, uint32_t>;

BusIndices BuildBusIndices(const TransportCatalogue& db) {
    BusIndices bus_indices;
    uint32_t index = 0;
    for (const Bus& bus : db.GetBuses()) {
        bus_indices[&bus] = index++;
    }
    return bus_indices;
}

transport_catalogue_proto::Catalogue SerializeTransportCatalogue(
    const TransportCatalogue& db,
    const StopIndices& stop_indices
) {
    transport_catalogue_proto::Catalogue db_proto;

    for (const Stop& stop : db.GetStops()) {
        transport_catalogue_proto::Stop& stop_proto = *db_proto.add_stops();
        stop_proto.set_name(stop.name);
        transport_catalogue_proto::Coordinates& position_proto = *stop_proto.mutable_position();
        position_proto.set_latitude(stop.position.lat);
        position_proto.set_longitude(stop.position.lng);
    }

    for (const Bus& bus : db.GetBuses()) {
        transport_catalogue_proto::Bus& bus_proto = *db_proto.add_buses();
        bus_proto.set_name(bus.name);
        for (const StopPtr stop_ptr : bus.stops) {
            bus_proto.add_stops(stop_indices.at(stop_ptr));
        }
        for (const StopPtr stop_ptr : bus.endpoints) {
            bus_proto.add_endpoints(stop_indices.at(stop_ptr));
        }
    }

    for (const auto [stop_pair, distance] : db.GetDistances()) {
        const auto [stop_from, stop_to] = stop_pair;
        transport_catalogue_proto::StopDistanceItem& item_proto = *db_proto.add_stop_distances();
        item_proto.set_from(stop_indices.at(stop_from));
        item_proto.set_to(stop_indices.at(stop_to));
        item_proto.set_distance(distance);
    }

    return db_proto;
}

template <typename Stops>
std::vector<StopPtr> DeserializeStops(
    const TransportCatalogue::StopPool& stop_pool,
    const Stops& proto_stops
) {
    std::vector<StopPtr> stops;
    stops.reserve(proto_stops.size());
    for (const uint32_t stop_index : proto_stops) {
        stops.push_back(&stop_pool.at(stop_index));
    }
    return stops;
}

TransportCatalogue DeserializeTransportCatalogue(transport_catalogue_proto::Catalogue&& proto) {
    TransportCatalogue db;

    for (transport_catalogue_proto::Stop& stop_proto : *proto.mutable_stops()) {
        const transport_catalogue_proto::Coordinates& position = stop_proto.position();
        db.AddStop(std::move(*stop_proto.mutable_name()), {position.latitude(), position.longitude()});
    }

    const auto& stop_pool = db.GetStops();

    for (transport_catalogue_proto::Bus& bus_proto : *proto.mutable_buses()) {
        db.AddBus(
            std::move(*bus_proto.mutable_name()),
            DeserializeStops(stop_pool, bus_proto.stops()),
            DeserializeStops(stop_pool, bus_proto.endpoints())
        );
    }

    for (const transport_catalogue_proto::StopDistanceItem& stop_distance_item : proto.stop_distances()) {
        db.SetDistance(
            &stop_pool.at(stop_distance_item.from()),
            &stop_pool.at(stop_distance_item.to()),
            stop_distance_item.distance()
        );
    }

    return db;
}

transport_catalogue_proto::RenderSettings SerializeRenderSettings(const renderer::RenderSettings& settings) {
    transport_catalogue_proto::RenderSettings proto;
    proto.set_max_width(settings.max_width);
    proto.set_max_height(settings.max_height);
    proto.set_padding(settings.padding);

    for (const svg::Color& color : settings.palette) {
        *proto.add_palette() = SerializeColor(color);
    }

    proto.set_line_width(settings.line_width);
    *proto.mutable_underlayer_color() = SerializeColor(settings.underlayer_color);
    proto.set_underlayer_width(settings.underlayer_width);
    proto.set_stop_radius(settings.stop_radius);

    *proto.mutable_bus_label_offset() = SerializePoint(settings.bus_label_offset);
    proto.set_bus_label_font_size(settings.bus_label_font_size);
    *proto.mutable_stop_label_offset() = SerializePoint(settings.stop_label_offset);
    proto.set_stop_label_font_size(settings.stop_label_font_size);

    return proto;
}

renderer::RenderSettings DeserializeRenderSettings(const transport_catalogue_proto::RenderSettings& proto) {
    renderer::RenderSettings settings;
    settings.max_width = proto.max_width();
    settings.max_height = proto.max_height();
    settings.padding = proto.padding();

    settings.palette.reserve(proto.palette_size());
    for (const auto& color : proto.palette()) {
        settings.palette.push_back(DeserializeColor(color));
    }

    settings.line_width = proto.line_width();
    settings.underlayer_color = DeserializeColor(proto.underlayer_color());
    settings.underlayer_width = proto.underlayer_width();
    settings.stop_radius = proto.stop_radius();

    settings.bus_label_offset = DeserializePoint(proto.bus_label_offset());
    settings.bus_label_font_size = proto.bus_label_font_size();
    settings.stop_label_offset = DeserializePoint(proto.stop_label_offset());
    settings.stop_label_font_size = proto.stop_label_font_size();

    return settings;
}

transport_catalogue_proto::MapRenderer SerializeMapRenderer(
    const renderer::MapRenderer& renderer,
    const StopIndices& stop_indices
) {
    transport_catalogue_proto::MapRenderer proto;
    *proto.mutable_render_settings() = SerializeRenderSettings(renderer.GetRenderSettings());

    for (const auto [stop_ptr, coords] : renderer.GetStopsCoords()) {
        auto& stop_coords_proto = *proto.add_stops_coords();
        stop_coords_proto.set_stop(stop_indices.at(stop_ptr));
        *stop_coords_proto.mutable_point() = SerializePoint(coords);
    }

    return proto;
}

renderer::MapRenderer DeserializeMapRenderer(
    const transport_catalogue_proto::MapRenderer& proto,
    const TransportCatalogue& db
) {
    renderer::MapRendererBuilder builder(DeserializeRenderSettings(proto.render_settings()), db);

    for (const auto& stop_coords_proto : proto.stops_coords()) {
        builder.SetStopCoords(
            &db.GetStops().at(stop_coords_proto.stop()),
            DeserializePoint(stop_coords_proto.point())
        );
    }

    return std::move(builder).Build();
}

double ConvertWeightToDouble(double source) {
    return source;
}

double ConvertWeightToDouble(router::Minutes source) {
    return source.count();
}

template <typename Weight>
Weight ConvertDoubleToWeight(double source) {
    return Weight{source};
}

template <typename Weight>
graph_proto::DirectedWeightedGraph SerializeGraph(const graph::DirectedWeightedGraph<Weight>& source_graph) {
    graph_proto::DirectedWeightedGraph proto;

    proto.set_vertex_count(source_graph.GetVertexCount());

    for (graph::EdgeId edge_id = 0; edge_id < source_graph.GetEdgeCount(); ++edge_id) {
        auto& edge_proto = *proto.add_edges();
        const auto& edge = source_graph.GetEdge(edge_id);
        edge_proto.set_from(edge.from);
        edge_proto.set_to(edge.to);
        edge_proto.set_weight(ConvertWeightToDouble(edge.weight));
    }

    return proto;
}

template <typename Weight>
graph::DirectedWeightedGraph<Weight> DeserializeGraph(const graph_proto::DirectedWeightedGraph& proto) {
    graph::DirectedWeightedGraph<Weight> target_graph(proto.vertex_count());

    for (const auto& edge_proto : proto.edges()) {
        graph::Edge<Weight> edge;
        edge.from = edge_proto.from();
        edge.to = edge_proto.to();
        edge.weight = ConvertDoubleToWeight<Weight>(edge_proto.weight());
        target_graph.AddEdge(edge);
    }

    return target_graph;
}

template <typename Weight>
graph_proto::Router SerializeRouter(const graph::Router<Weight>& router) {
    const auto& graph = router.GetGraph();
    const size_t vertex_count = graph.GetVertexCount();
    graph_proto::Router proto;

    for (graph::VertexId from = 0; from < vertex_count; ++from) {
        auto& source_data_proto = *proto.add_sources_data();
        for (graph::VertexId to = 0; to < vertex_count; ++to) {
            auto& route_data_proto = *source_data_proto.add_targets_data();
            const auto* route_data = router.GetRouteInternalData(from, to);
            if (route_data) {
                route_data_proto.set_exists(true);
                route_data_proto.set_weight(ConvertWeightToDouble(route_data->weight));
                if (route_data->prev_edge) {
                    route_data_proto.set_has_prev_edge(true);
                    route_data_proto.set_prev_edge(*route_data->prev_edge);
                }
            }
        }
    }

    return proto;
}

template <typename Weight>
graph::Router<Weight> DeserializeRouter(const graph_proto::Router& proto, const graph::DirectedWeightedGraph<Weight>& target_graph) {
    const size_t vertex_count = target_graph.GetVertexCount();
    graph::RouterBuilder<Weight> builder(target_graph);

    for (graph::VertexId from = 0; from < vertex_count; ++from) {
        const auto& source_data_proto = proto.sources_data(from);
        for (graph::VertexId to = 0; to < vertex_count; ++to) {
            const auto& route_data_proto = source_data_proto.targets_data(to);
            if (route_data_proto.exists()) {
                typename graph::Router<Weight>::RouteInternalData route_data{
                    ConvertDoubleToWeight<Weight>(route_data_proto.weight()),
                    std::nullopt
                };
                if (route_data_proto.has_prev_edge()) {
                    route_data.prev_edge = route_data_proto.prev_edge();
                }
                builder.SetRouteInternalData(from, to, route_data);
            }
        }
    }

    return std::move(builder).Build();
}

transport_catalogue_proto::RoutingSettings SerializeRoutingSettings(const router::RoutingSettings& settings) {
    transport_catalogue_proto::RoutingSettings proto;
    proto.set_bus_wait_time_minutes(settings.bus_wait_time.count());
    proto.set_bus_velocity_kmh(settings.bus_velocity_kmh);
    return proto;
}

router::RoutingSettings DeserializeRoutingSettings(const transport_catalogue_proto::RoutingSettings& proto) {
    router::RoutingSettings settings;
    settings.bus_wait_time = std::chrono::minutes(proto.bus_wait_time_minutes());
    settings.bus_velocity_kmh = proto.bus_velocity_kmh();
    return settings;
}

transport_catalogue_proto::TransportRouter SerializeTransportRouter(
    const router::Router& transport_router,
    const StopIndices& stop_indices,
    const BusIndices& bus_indices
) {
    transport_catalogue_proto::TransportRouter proto;

    *proto.mutable_routing_settings() = SerializeRoutingSettings(transport_router.GetRoutingSettings());

    const auto& graph = transport_router.GetGraph();

    *proto.mutable_graph() = SerializeGraph(graph);
    *proto.mutable_router() = SerializeRouter(transport_router.GetRouter());

    for (const auto [stop_ptr, stop_idx] : stop_indices) {
        auto& vertex_ids_proto = *proto.add_stops_vertex_ids();
        const auto& vertex_ids = transport_router.GetStopVertexIds(stop_ptr);
        vertex_ids_proto.set_stop(stop_idx);
        vertex_ids_proto.set_in(vertex_ids.in);
        vertex_ids_proto.set_out(vertex_ids.out);
    }

    for (graph::VertexId vertex = 0; vertex < graph.GetVertexCount(); ++vertex) {
        proto.add_vertices_info()->set_stop(stop_indices.at(transport_router.GetVertexInfo(vertex).stop_ptr));
    }

    for (graph::EdgeId edge = 0; edge < graph.GetEdgeCount(); ++edge) {
        auto& edge_info_proto = *proto.add_edges_info();
        const auto& edge_info = transport_router.GetEdgeInfo(edge);
        if (std::holds_alternative<router::Router::BusEdgeInfo>(edge_info)) {
            const auto& bus_edge_info = std::get<router::Router::BusEdgeInfo>(edge_info);
            auto& bus_edge_info_proto = *edge_info_proto.mutable_bus_data();
            bus_edge_info_proto.set_bus(bus_indices.at(bus_edge_info.bus_ptr));
            bus_edge_info_proto.set_span_count(bus_edge_info.span_count);
        } else {
            edge_info_proto.mutable_wait_data();
        }
    }

    return proto;
}

router::Router DeserializeTransportRouter(const transport_catalogue_proto::TransportRouter& proto, const TransportCatalogue& db) {
    auto routing_settings = DeserializeRoutingSettings(proto.routing_settings());

    using Weight = router::Minutes;
    auto bus_graph = DeserializeGraph<Weight>(proto.graph());
    router::RouterBuilder router_builder(std::move(routing_settings), std::move(bus_graph));

    auto internal_router = DeserializeRouter<Weight>(proto.router(), router_builder.GetGraph());
    router_builder.SetInternalRouter(std::move(internal_router));


    const auto& stops = db.GetStops();
    const auto& buses = db.GetBuses();

    for (const auto& stop_vertex_ids_proto : proto.stops_vertex_ids()) {
        router_builder.SetStopVertexIds(
            &stops.at(stop_vertex_ids_proto.stop()),
            {
                stop_vertex_ids_proto.in(),
                stop_vertex_ids_proto.out(),
            }
        );
    }

    graph::VertexId vertex = 0;
    for (const auto& vertex_info_proto : proto.vertices_info()) {
        router_builder.SetVertexInfo(vertex++, {&stops.at(vertex_info_proto.stop())});
    }

    graph::EdgeId edge = 0;
    for (const auto& edge_info_proto : proto.edges_info()) {
        if (edge_info_proto.has_bus_data()) {
            const auto& bus_info_proto = edge_info_proto.bus_data();
            router_builder.SetEdgeInfo(
                edge,
                router::Router::BusEdgeInfo{
                    &buses.at(bus_info_proto.bus()),
                    bus_info_proto.span_count(),
                }
            );
        } else {
            router_builder.SetEdgeInfo(edge, router::Router::WaitEdgeInfo{});
        }
        ++edge;
    }

    return std::move(router_builder).Build();
}

}  // namespace


transport_catalogue_proto::TransportCatalogue SerializeUniverse(
    const TransportCatalogue& db,
    const renderer::MapRenderer& renderer,
    const router::Router& router
) {
    transport_catalogue_proto::TransportCatalogue proto;

    const auto stop_indices = BuildStopIndices(db);
    const auto bus_indices = BuildBusIndices(db);
    *proto.mutable_catalogue() = SerializeTransportCatalogue(db, stop_indices);
    *proto.mutable_renderer() = SerializeMapRenderer(renderer, stop_indices);
    *proto.mutable_router() = SerializeTransportRouter(router, stop_indices, bus_indices);

    return proto;
}

std::tuple<TransportCatalogue, renderer::MapRenderer, router::Router> DeserializeUniverse(
    transport_catalogue_proto::TransportCatalogue&& proto
) {
    TransportCatalogue db = DeserializeTransportCatalogue(std::move(*proto.mutable_catalogue()));
    renderer::MapRenderer renderer = DeserializeMapRenderer(proto.renderer(), db);
    router::Router transport_router = DeserializeTransportRouter(proto.router(), db);
    return {std::move(db), std::move(renderer), std::move(transport_router)};
}

}  // namespace transport_catalogue::serialization