#pragma once

#include "transport_catalogue.h"
#include "domain.h"
#include "router.h"
#include "graph.h"

#include "log_duration.h"

#include <memory>

namespace TRouter {

using namespace graph;
using namespace Catalogue;
using namespace domain;

using namespace std::literals;

enum class RouteReqestType {
	NONE,
	WAIT,
	BUS
};

//class RouteItem {
//public:
//
//
//private:
//	RouteReqestType type = RouteReqestType::NONE;
//	double time = .0;
//};
//
//class RouteWait : public RouteItem {
//public:
//
//private:
//	std::optional<std::string_view> stop_name;
//};
//
//class RouteWait : public RouteItem {
//public:
//
//private:
//	std::optional<std::string_view> bus_name;
//	std::optional<size_t> span_count;
//};

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
	using Vertex_Ids = std::unordered_map<const Stop*, VertexId>;
	using EdgeBusSpan = std::unordered_map<std::pair<VertexId, VertexId>, std::pair<const Bus*, size_t>, HacherPair>;

	TransportRouter(std::unique_ptr<graph>&& graph, std::unique_ptr<Vertex_Ids>&& vertex_ids, 
		std::unique_ptr<EdgeBusSpan>&& span_counts,
		std::unique_ptr<std::unordered_map<VertexId, const Stop*>>&& id_stop,
		const Catalogue::TransportCatalogue& ts,
		const domain::RoutingSettings routing_settings)
		: graph_(std::move(graph)), 
		stop_to_vertex(std::move(vertex_ids)),
		edge_to_bus_span(std::move(span_counts)),
		id_to_stop(std::move(id_stop)),
		ts_(ts), 
		routing_settings_(routing_settings)
	{
	}

	std::optional<const RouteInfo> GetRouteInfo(std::string_view from, std::string_view to){
		LOG_DURATION("GetRouteInfo");
		Router router(*graph_);
		const auto& route_info = router.BuildRoute((*stop_to_vertex)[ts_.FindStop(from)], (*stop_to_vertex)[ts_.FindStop(to)]);

		if (!route_info) {
			return std::nullopt;
		}

		RouteInfo info;
		info.total_time = (*route_info).weight;
		info.items.reserve((*route_info).edges.size());

		for (const auto& item : (*route_info).edges) {
			const auto& edge = (*graph_).GetEdge(item);

			RouteItem item_wait;
			item_wait.type = RouteReqestType::WAIT;
			item_wait.time = routing_settings_.bus_wait_time_;
			item_wait.stop_name = (*id_to_stop)[edge.from]->name_;

			info.items.push_back(std::move(item_wait));

			RouteItem item_bus;

			item_bus.type = RouteReqestType::BUS;
			item_bus.time = edge.weight - routing_settings_.bus_wait_time_;
			item_bus.bus_name = (*edge_to_bus_span)[{edge.from, edge.to}].first->name_;
			item_bus.span_count = (*edge_to_bus_span)[{edge.from, edge.to}].second;

			info.items.push_back(std::move(item_bus));
		}

		return info;
	}

private:
	
	std::unique_ptr<graph> graph_;

	std::unique_ptr<Vertex_Ids> stop_to_vertex;
	std::unique_ptr<EdgeBusSpan> edge_to_bus_span;
	std::unique_ptr<std::unordered_map<VertexId, const Stop*>> id_to_stop;

	const Catalogue::TransportCatalogue& ts_;
	const domain::RoutingSettings routing_settings_;
};

} // namespace Router