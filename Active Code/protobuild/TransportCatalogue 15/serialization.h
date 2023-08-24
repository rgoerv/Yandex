#pragma once

#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"
#include "transport_router.h"
#include "graph.h"
#include "json_reader.h"

#include <memory>

#include <svg.pb.h>
#include <graph.pb.h>
#include <transport_router.pb.h>
#include <map_renderer.pb.h>
#include <transport_catalogue.pb.h>

namespace serial {

using namespace graph;
using namespace domain;

struct SerializationSettings {
    SerializationSettings(std::string file_) 
        : file(file_) 
    {
    }
    std::string file; 
};

using Stop_VertexId = std::unordered_map<const Stop*, size_t>;
using VertexId_Stop = std::unordered_map<size_t, const Stop*>;
using Edge_BusSpan = std::unordered_map<std::pair<size_t, size_t>, std::pair<const Bus*, size_t>, HacherPair>;

class SerialTC {
public:
    using graph = DirectedWeightedGraph<double>;
    SerialTC() = default;

    bool Serialization(const Catalogue::TransportCatalogue& catalogue);
    bool Deserialization(Catalogue::TransportCatalogue& catalogue);

    void SetSerializationSettings(SerializationSettings&& ss);
    const SerializationSettings& GetSerializationSettings() const;

    void SetRenderSettings(renderer::RenderSettings&& rs);
    const renderer::RenderSettings& GetRenderSettings() const;

    void SetRoutingSettings(TRouter::RoutingSettings&& rs);
    const TRouter::RoutingSettings& GetRoutingSettings() const;

    void SetGraph(graph&& graph_ptr_);
    graph&& GetGraph();

    void SetStopToVertex(Stop_VertexId&& stop_to_vertex_);
    Stop_VertexId&& GetStopToVertex();

    void SetIdToStop(VertexId_Stop&& id_to_stop_);
    VertexId_Stop&& GetIdToStop();

    void SetEdgeToBusSpan(Edge_BusSpan&& edge_to_bus_span_);
    Edge_BusSpan&& GetEdgeToBusSpan();

private:
    std::unique_ptr<SerializationSettings> serialization_settings = nullptr;
    std::unique_ptr<renderer::RenderSettings> render_settings = nullptr;
    std::unique_ptr<TRouter::RoutingSettings> routing_settings = nullptr;
    std::unique_ptr<graph> graph_ptr = nullptr;
    std::unique_ptr<Stop_VertexId> stop_to_vertex = nullptr;
    std::unique_ptr<VertexId_Stop> id_to_stop = nullptr;
    std::unique_ptr<Edge_BusSpan> edge_to_bus_span = nullptr;

    void SerializeCatalogue(const Catalogue::TransportCatalogue& catalogue, 
                                proto_tc::TransportCatalogue& pb_catalogue);
    void SerializeRenderSettings(proto_map_render::RenderSettings& pb_render_settings);
    void SerializeRouter(proto_tr::TransportRouter& router);


    void DeserializeTransportRouter(const proto_tr::TransportRouter& pb_router, const Catalogue::TransportCatalogue& catalogue);
    void DeserializeCatalogue(const proto_tc::TransportCatalogue& pb_catalogue, Catalogue::TransportCatalogue& catalogue);
    void DeserializeRenderSettings(const proto_map_render::RenderSettings& pb_render_set, renderer::RenderSettings& rs);

    
};

} // namespace serial