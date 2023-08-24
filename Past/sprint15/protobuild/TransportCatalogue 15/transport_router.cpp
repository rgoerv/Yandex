#include "transport_router.h"
#include "json_reader.h"
#include "graph.h"
#include "router.h"

#include <memory>
#include <optional>

namespace TRouter {

std::optional<const RouteInfo> TransportRouter::GetRouteInfo(std::string_view from, std::string_view to) const {

	const auto& route_info = (*router_).BuildRoute((*stop_to_vertex)[ts_.FindStop(from)], (*stop_to_vertex)[ts_.FindStop(to)]);
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
		item_wait.stop_name = (*vertex_to_stop)[edge.from]->name_;

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

} // namespace TRouter