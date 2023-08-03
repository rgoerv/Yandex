#pragma once

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

#include "transport_catalogue.h"
#include "transport_router.h"
#include "geo.h"
#include "domain.h"
#include "json.h"
#include "graph.h"

#include <utility>
#include <variant>

namespace JsonReader {

using namespace std::literals;
using namespace Catalogue;
using namespace json;
using namespace domain;
using namespace graph;
using namespace TRouter;

using Vertex_Ids = std::unordered_map<const Stop*, VertexId>;
using EdgeBusSpan = std::unordered_map<std::pair<VertexId, VertexId>, std::pair<const Bus*, size_t>, HacherPair>;

class Reader {
public:
    using graph = DirectedWeightedGraph<double>;
    using SpanCount = size_t;

    Reader(std::istream& input);
    void Reply(std::ostream& output) const;

    const TransportCatalogue& GetCatalogue() const;
    const json::Document GetRenderSettings() const;
    const domain::RoutingSettings GetRoutingSettings() const;

private:
    json::Document document;
    TransportCatalogue catalogue;
    std::unordered_map<std::pair<std::string_view, std::string_view>, int64_t, HacherPair> stops_to_dstns;
    Array stat_response;

    Vertex_Ids stop_to_vertex;
    EdgeBusSpan edge_to_bus_span;

    std::unordered_map<VertexId, const Stop*> id_to_stop;

    void BaseRequestHandle();
    void StopBaseRequestHandle(const Array& base_requests);
    void BusBaseRequestHandle(const Array& base_reauest);
    void AgreeDistances(const std::vector<const domain::Stop*>& stops, int64_t& lenght, double& geo_length) const;

    void StatRequestHandle();
    void StopStatRequestHandle(const Node& request);
    void BusStatRequestHandle(const Node& request);
    void MapStatRequestHandle(const Node& request);
    void RouterStatRequestHandle(const Node& request, const graph& graph_);

    void BuildGraph(graph& graph_);
};

} // namespace JsonReader