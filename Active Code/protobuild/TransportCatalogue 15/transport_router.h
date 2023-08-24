#pragma once

#include "transport_catalogue.h"
#include "domain.h"
#include "router.h"
#include "graph.h"

#include <memory>
#include <unordered_map>

namespace TRouter {

using namespace graph;
using namespace Catalogue;
using namespace domain;

using namespace std::literals;

using Stop_VertexId = std::unordered_map<const Stop*, VertexId>;
using VertexId_Stop = std::unordered_map<VertexId, const Stop*>;
using Edge_BusSpan = std::unordered_map<std::pair<VertexId, VertexId>, std::pair<const Bus*, size_t>, HacherPair>;

struct RoutingSettings {
    double bus_wait_time_ = .0;
    double bus_velocity_ = .0;
};

enum class RouteReqestType {
	NONE,
	WAIT,
	BUS
};

// need class structure, with general class and two classes with relevant request type
// in TRouter::RouteInfo elements type in vector - general class 
struct RouteItem {
	RouteReqestType type = RouteReqestType::NONE;
	double time = .0;

	std::optional<std::string_view> stop_name;

	std::optional<std::string_view> bus_name;
	std::optional<size_t> span_count;
};

struct RouteInfo 
{
	double total_time = .0;
	std::vector<RouteItem> items;
};

class TransportRouter {
public:
	using graph = DirectedWeightedGraph<double>;

	TransportRouter(std::unique_ptr<graph>&& graph, 
		std::unique_ptr<Router<double>>&& router, 
		std::unique_ptr<Stop_VertexId>&& vertex_ids,
		std::unique_ptr<Edge_BusSpan>&& span_counts,
		std::unique_ptr<VertexId_Stop>&& id_stop,
		const Catalogue::TransportCatalogue& ts,
		const RoutingSettings routing_settings)
		: graph_(std::move(graph)),
		router_(std::move(router)),
		stop_to_vertex(std::move(vertex_ids)),
		edge_to_bus_span(std::move(span_counts)),
		vertex_to_stop(std::move(id_stop)),
		ts_(ts), 
		routing_settings_(routing_settings)
	{
	}

	std::optional<const RouteInfo> GetRouteInfo(std::string_view from, std::string_view to) const;

private:
	std::unique_ptr<graph> graph_ = nullptr;
	std::unique_ptr<Router<double>> router_ = nullptr;

	std::unique_ptr<Stop_VertexId> stop_to_vertex = nullptr;
	std::unique_ptr<VertexId_Stop> vertex_to_stop = nullptr;

	std::unique_ptr<Edge_BusSpan> edge_to_bus_span = nullptr;

	const Catalogue::TransportCatalogue& ts_;
	const RoutingSettings routing_settings_;
};

} // namespace TRouter