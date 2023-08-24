#pragma once
#include "json.h"

#include <stdexcept>

namespace transport_catalogue {

namespace service {
class RequestHandler;
}  // namespace service

namespace renderer {
struct RenderSettings;
}  // namespace renderer

namespace router {
struct RoutingSettings;
}  // namespace router

class TransportCatalogue;

namespace json_reader {

class JsonReaderError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class InvalidRequestError : public JsonReaderError {
public:
    using JsonReaderError::JsonReaderError;
};

TransportCatalogue ReadTransportCatalogue(const json::Array& base_requests_json);

renderer::RenderSettings ReadRenderSettings(const json::Dict& render_settings_json);

router::RoutingSettings ReadRoutingSettings(const json::Dict& routing_settings_json);

json::Array HandleRequests(const json::Array& requests_json,
                           const service::RequestHandler& handler);

}  // namespace json_reader
}  // namespace transport_catalogue