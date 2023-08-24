#include "request_handler.h"

#include "map_renderer.h"
#include "transport_catalogue.h"
#include "transport_router.h"

namespace transport_catalogue::service {

RequestHandler::RequestHandler(
    const TransportCatalogue& db,
    const renderer::MapRenderer& renderer,
    const router::Router& router
)
    : db_(db)
    , renderer_(renderer)
    , router_(router) {
}

std::optional<BusStat> RequestHandler::GetBusStat(std::string_view bus_name) const {
    const BusPtr bus = db_.FindBus(bus_name);
    return bus ? std::make_optional(db_.GetStat(bus)) : std::nullopt;
}

const std::unordered_set<BusPtr>* RequestHandler::GetBusesByStop(std::string_view stop_name) const {
    const StopPtr stop = db_.FindStop(stop_name);
    return stop ? &(db_.GetBusesByStop(stop)) : nullptr;
}

svg::Document RequestHandler::RenderMap() const {
    svg::Document doc;
    renderer_.Draw(doc);
    return doc;
}

std::optional<router::RouteInfo> RequestHandler::FindRoute(std::string_view stop_name_from, std::string_view stop_name_to) const {
    const StopPtr stop_from = db_.FindStop(stop_name_from);
    const StopPtr stop_to = db_.FindStop(stop_name_to);
    if (stop_from && stop_to) {
        return router_.FindRoute(stop_from, stop_to);
    } else {
        return std::nullopt;
    }
}

}  // namespace transport_catalogue::service