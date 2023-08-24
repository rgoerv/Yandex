#pragma once
#include "domain.h"

#include <optional>
#include <string_view>
#include <unordered_set>

namespace svg {
class Document;
}

namespace transport_catalogue {

namespace renderer {
class MapRenderer;
}

namespace router {
struct RouteInfo;
class Router;
}

class TransportCatalogue;
struct BusStat;

namespace service {

class RequestHandler {
public:
    RequestHandler(
        const TransportCatalogue& db, 
        const renderer::MapRenderer& renderer,
        const router::Router& router
    );

    std::optional<BusStat> GetBusStat(std::string_view bus_name) const;

    const std::unordered_set<BusPtr>* GetBusesByStop(std::string_view stop_name) const;

    svg::Document RenderMap() const;

    std::optional<router::RouteInfo> FindRoute(std::string_view stop_from, std::string_view stop_to) const;

private:
    const TransportCatalogue& db_;
    const renderer::MapRenderer& renderer_;
    const router::Router& router_;
};

}  // namespace service
}  // namespace transport_catalogue