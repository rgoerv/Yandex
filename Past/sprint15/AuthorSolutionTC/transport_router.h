#pragma once

#include "domain.h"
#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"

#include <chrono>
#include <memory>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

namespace transport_catalogue::router {

struct RoutingSettings {
    std::chrono::minutes bus_wait_time;
    double bus_velocity_kmh;
};

using Minutes = std::chrono::duration<double, std::chrono::minutes::period>;

struct RouteInfo {
    Minutes total_time;

    struct BusItem {
        BusPtr bus_ptr;
        Minutes time;
        size_t span_count;
    };
    struct WaitItem {
        StopPtr stop_ptr;
        Minutes time;
    };

    using Item = std::variant<BusItem, WaitItem>;
    std::vector<Item> items;
};

class RouterBuilder;

class Router {
public:
    Router(RoutingSettings settings, const TransportCatalogue& db);

    std::optional<RouteInfo> FindRoute(StopPtr stop_from, StopPtr stop_to) const;

    const RoutingSettings& GetRoutingSettings() const {
        return settings_;
    }

    const auto& GetGraph() const {
        return *graph_holder_;
    }

    const auto& GetRouter() const {
        return *router_holder_;
    }

    struct StopVertexIds {
        graph::VertexId in;
        graph::VertexId out;
    };
    const StopVertexIds& GetStopVertexIds(StopPtr stop) const {
        return stops_vertex_ids_.at(stop);
    }

    struct VertexInfo {
        StopPtr stop_ptr;
    };

    const VertexInfo& GetVertexInfo(graph::VertexId vertex) const {
        return vertices_info_.at(vertex);
    }

    struct BusEdgeInfo {
        BusPtr bus_ptr;
        size_t span_count;
    };
    struct WaitEdgeInfo {};
    using EdgeInfo = std::variant<BusEdgeInfo, WaitEdgeInfo>;
    
    const EdgeInfo& GetEdgeInfo(graph::EdgeId edge) const {
        return edges_info_.at(edge);
    }

private:
    using BusGraph = graph::DirectedWeightedGraph<Minutes>;
    using InternalRouter = graph::Router<Minutes>;

    Router(RoutingSettings settings, BusGraph graph)
        : settings_(settings)
        , graph_holder_(std::make_unique<BusGraph>(graph))
        , vertices_info_(graph.GetVertexCount())
        , edges_info_(graph.GetEdgeCount())
    {
    }
    
    void FillGraphWithStops(const TransportCatalogue::StopPool& stops);
    
    void FillGraphWithBuses(const TransportCatalogue& db);

    RoutingSettings settings_;

    std::unique_ptr<BusGraph> graph_holder_;  // store indirectly for safe referencing from router
    std::unique_ptr<InternalRouter> router_holder_;
    std::unordered_map<StopPtr, StopVertexIds> stops_vertex_ids_;
    std::vector<VertexInfo> vertices_info_;
    std::vector<EdgeInfo> edges_info_;

    friend class RouterBuilder;
};

class RouterBuilder {
public:
    RouterBuilder(
        RoutingSettings settings,
        Router::BusGraph graph
    )
        : router_(std::move(settings), std::move(graph))
    {}

    const auto& GetGraph() const {
        return router_.GetGraph();
    }

    void SetInternalRouter(Router::InternalRouter internal_router) {
        router_.router_holder_ = std::make_unique<Router::InternalRouter>(std::move(internal_router));
    }

    void SetStopVertexIds(StopPtr stop, const Router::StopVertexIds& ids) {
        router_.stops_vertex_ids_[stop] = ids;
    }

    void SetVertexInfo(graph::VertexId vertex, const Router::VertexInfo& info) {
        router_.vertices_info_.at(vertex) = info;
    }

    template <typename EdgeInfo>
    void SetEdgeInfo(graph::EdgeId edge, const EdgeInfo& info) {
        router_.edges_info_.at(edge) = info;
    }

    Router Build() && {
        return std::move(router_);
    }

private:
    Router router_;
};

}  // namespace transport_catalogue::router