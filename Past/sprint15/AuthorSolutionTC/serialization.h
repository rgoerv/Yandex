#include <transport_catalogue.pb.h>
#include <tuple>

namespace transport_catalogue {

class TransportCatalogue;

namespace renderer {
class MapRenderer;
}  // namespace renderer

namespace router {
class Router;
}  // namespace renderer

namespace serialization {

transport_catalogue_proto::TransportCatalogue SerializeUniverse(
    const TransportCatalogue& db,
    const renderer::MapRenderer& renderer,
    const router::Router& router);

std::tuple<TransportCatalogue, renderer::MapRenderer, router::Router> DeserializeUniverse(
    transport_catalogue_proto::TransportCatalogue&& proto);

}  // namespace serialization
}  // namespace transport_catalogue