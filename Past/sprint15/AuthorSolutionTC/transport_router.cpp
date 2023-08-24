#include "transport_router.h"

namespace transport_catalogue::router {

Router::Router(RoutingSettings settings, const TransportCatalogue& db)
    : settings_(std::move(settings))
{
    const auto& stops = db.GetStops();
    const size_t vertex_count = stops.size() * 2;

    graph_holder_ = std::make_unique<BusGraph>(vertex_count);
    vertices_info_.resize(vertex_count);
    
    FillGraphWithStops(stops);
    FillGraphWithBuses(db);
    
    router_holder_ = std::make_unique<InternalRouter>(*graph_holder_);
}

std::optional<RouteInfo> Router::FindRoute(StopPtr stop_from, StopPtr stop_to) const {
    const graph::VertexId vertex_from = stops_vertex_ids_.at(stop_from).out;
    const graph::VertexId vertex_to = stops_vertex_ids_.at(stop_to).out;
    const auto route = router_holder_->BuildRoute(vertex_from, vertex_to);
    if (!route) {
        return std::nullopt;
    }

    RouteInfo route_info;
    route_info.total_time = route->weight;
    route_info.items.reserve(route->edges.size());
    for (const graph::EdgeId edge_id : route->edges) {
        const auto& edge = graph_holder_->GetEdge(edge_id);
        const auto& edge_info = edges_info_[edge_id];
        if (std::holds_alternative<BusEdgeInfo>(edge_info)) {
            const BusEdgeInfo& bus_edge_info = std::get<BusEdgeInfo>(edge_info);
            route_info.items.push_back(RouteInfo::BusItem{
                /* bus_ptr */    bus_edge_info.bus_ptr,
                /* time */       edge.weight,
                /* span_count */ bus_edge_info.span_count,
            });
        } else {
            const graph::VertexId vertex_id = edge.from;
            route_info.items.push_back(RouteInfo::WaitItem{
                /* stop_ptr */ vertices_info_[vertex_id].stop_ptr,
                /* time */     edge.weight,
            });
        }
    }

    return route_info;
}

void Router::FillGraphWithStops(const TransportCatalogue::StopPool& stops) {
    graph::VertexId vertex_id = 0;

    for (const Stop& stop : stops) {
        auto& vertex_ids = stops_vertex_ids_[&stop];
        vertex_ids.in = vertex_id++;
        vertex_ids.out = vertex_id++;
        vertices_info_[vertex_ids.in] = {&stop};
        vertices_info_[vertex_ids.out] = {&stop};
    
        edges_info_.push_back(WaitEdgeInfo{});
        const graph::EdgeId edge_id = graph_holder_->AddEdge({
            vertex_ids.out,
            vertex_ids.in,
            Minutes(settings_.bus_wait_time.count())
        });
        assert(edge_id == edges_info_.size() - 1);
    }

    assert(vertex_id == graph_holder_->GetVertexCount());
}
    
void Router::FillGraphWithBuses(const TransportCatalogue& db) {
    for (const Bus& bus : db.GetBuses()) {
        const auto& bus_stops = bus.stops;
        const size_t stop_count = bus_stops.size();
        if (stop_count <= 1) {
            continue;
        }
        auto compute_distance_from = [&db, &bus_stops](size_t stop_idx) {
            return db.GetDistance(bus_stops[stop_idx], bus_stops[stop_idx + 1]);
        };
        for (size_t start_stop_idx = 0; start_stop_idx + 1 < stop_count; ++start_stop_idx) {
            const graph::VertexId start_vertex = stops_vertex_ids_.at(bus_stops[start_stop_idx]).in;
            int total_distance = 0;
            for (size_t finish_stop_idx = start_stop_idx + 1; finish_stop_idx < stop_count; ++finish_stop_idx) {
                total_distance += compute_distance_from(finish_stop_idx - 1);
                edges_info_.push_back(BusEdgeInfo{
                    &bus,
                    finish_stop_idx - start_stop_idx,
                });
                const graph::EdgeId edge_id = graph_holder_->AddEdge({
                    start_vertex,
                    stops_vertex_ids_.at(bus_stops[finish_stop_idx]).out,
                    Minutes(
                        total_distance * 1.0 / (settings_.bus_velocity_kmh * 1000.0 / 60)
                    )  // m / (km/h * 1000 / 60) = min
                });
                assert(edge_id == edges_info_.size() - 1);
            }
        }
    }
}

}  // namespace transport_catalogue::router