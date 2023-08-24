/* MATCHER: SuccessMatcher */
#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "serialization.h"
#include "transport_catalogue.h"
#include "transport_router.h"

#include <transport_catalogue.pb.h>

#include <fstream>
#include <iostream>
#include <string_view>

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

const std::string& GetBaseFileName(const json::Dict& data) {
    return data.at("serialization_settings"s).AsDict().at("file"s).AsString();
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    try {
        using namespace transport_catalogue;

        const auto json = json::Load(std::cin).GetRoot().AsDict();

        if (mode == "make_base"sv) {

            const auto db = json_reader::ReadTransportCatalogue(json.at("base_requests"s).AsArray());

            renderer::RenderSettings render_settings
                = json_reader::ReadRenderSettings(json.at("render_settings"s).AsDict());
            const renderer::MapRenderer renderer{std::move(render_settings), db};

            router::RoutingSettings routing_settings
                = json_reader::ReadRoutingSettings(json.at("routing_settings"s).AsDict());
            const router::Router router{std::move(routing_settings), db};

            const std::string& file_name = GetBaseFileName(json);
            std::ofstream file(file_name, std::ios::binary);
            assert(serialization::SerializeUniverse(db, renderer, router).SerializeToOstream(&file));

        } else if (mode == "process_requests"sv) {

            const std::string& file_name = GetBaseFileName(json);
            std::ifstream file(file_name, std::ios::binary);
            transport_catalogue_proto::TransportCatalogue proto;
            assert(proto.ParseFromIstream(&file));

            const auto [db, renderer, router] = serialization::DeserializeUniverse(std::move(proto));

            service::RequestHandler handler(db, renderer, router);

            json::Array responses
                = json_reader::HandleRequests(json.at("stat_requests"s).AsArray(), handler);

            json::Print(json::Document(std::move(responses)), std::cout);

        } else {

            PrintUsage();
            return 1;

        }

    } catch (const std::exception& e) {
        std::cerr << "Error: "sv << e.what() << std::endl;
    }
}