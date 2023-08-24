#pragma once

#include "transport_catalogue.h"
#include "transport_router.h"
#include "map_renderer.h"
#include "geo.h"
#include "domain.h"
#include "json.h"
#include "graph.h"

#include <utility>
#include <variant>
#include <unordered_map>
#include <memory>

namespace JsonReader {

using namespace std::literals;
using namespace Catalogue;
using namespace json;
using namespace domain;
using namespace graph;
using namespace TRouter;

using Stop_VertexId = std::unordered_map<const Stop*, VertexId>;
using VertexId_Stop = std::unordered_map<VertexId, const Stop*>;
using Edge_BusSpan = std::unordered_map<std::pair<VertexId, VertexId>, std::pair<const Bus*, size_t>, HacherPair>;

class Reader {
public:
    using graph = DirectedWeightedGraph<double>;

    Reader(std::istream& input);
    void MakeBase();
    void ProcessRequests();

    const TransportCatalogue& GetCatalogue() const;
    const renderer::RenderSettings& GetRenderSettings() const;
    TRouter::RoutingSettings GetRoutingSettings() const;

private:
    json::Document document;
    renderer::RenderSettings render_settings;
    TRouter::RoutingSettings routing_settings;

    TransportCatalogue catalogue;
    std::unordered_map<std::pair<std::string_view, std::string_view>, int64_t, HacherPair> stops_to_dstns;
    Array stat_response;

    std::unique_ptr<TRouter::TransportRouter> router;

    Stop_VertexId stop_to_vertex;
    VertexId_Stop id_to_stop;

    Edge_BusSpan edge_to_bus_span;

    void Reply(std::ostream& output) const;

    void BaseRequestHandle();
    void StopBaseRequestHandle(const Array& base_requests);
    void BusBaseRequestHandle(const Array& base_reauest);
    void AgreeDistances(const std::vector<const domain::Stop*>& stops, int64_t& lenght, double& geo_length) const;

    void StatRequestHandle();
    void StopStatRequestHandle(const Node& request);
    void BusStatRequestHandle(const Node& request);
    void MapStatRequestHandle(const Node& request);
    void RouterStatRequestHandle(const Node& request);

    void BuildGraph(graph& graph_);
    void BuildTransportRouter(graph& graph_);
};

} // namespace JsonReader