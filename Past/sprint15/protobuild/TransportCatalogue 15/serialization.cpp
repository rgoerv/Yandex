#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"
#include "transport_router.h"
#include "serialization.h"
#include "json_reader.h"

#include <svg.pb.h>
#include <graph.pb.h>
#include <transport_router.pb.h>
#include <map_renderer.pb.h>
#include <transport_catalogue.pb.h>

#include <fstream>
#include <filesystem>
#include <memory>

#include <iostream>

namespace serial {

void SerialTC::SetSerializationSettings(SerializationSettings&& ss) {
    serialization_settings = std::make_unique<SerializationSettings>(std::forward<SerializationSettings>(ss));
}

const SerializationSettings& SerialTC::GetSerializationSettings() const {
    assert(serialization_settings);
    return *serialization_settings;
}

void SerialTC::SetRenderSettings(renderer::RenderSettings&& rs) {
    render_settings = std::make_unique<renderer::RenderSettings>(std::forward<renderer::RenderSettings>(rs));
}

const renderer::RenderSettings& SerialTC::GetRenderSettings() const {
    assert(render_settings);
    return *render_settings;
}

void SerialTC::SetRoutingSettings(TRouter::RoutingSettings&& rs) {
    routing_settings = std::make_unique<TRouter::RoutingSettings>(std::forward<TRouter::RoutingSettings>(rs));
}

const TRouter::RoutingSettings& SerialTC::GetRoutingSettings() const {
    assert(routing_settings);
    return *routing_settings;
}

void SerialTC::SetGraph(graph&& graph_ptr_) {
    graph_ptr = std::move(std::make_unique<graph>(std::forward<graph>(graph_ptr_)));
}

SerialTC::graph&& SerialTC::GetGraph() {
    assert(graph_ptr);
    return std::move(*graph_ptr.release());
}

void SerialTC::SetStopToVertex(Stop_VertexId&& stop_to_vertex_){
    stop_to_vertex = std::move(std::make_unique<Stop_VertexId>(std::forward<Stop_VertexId>(stop_to_vertex_)));
}

Stop_VertexId&& SerialTC::GetStopToVertex() {
    assert(stop_to_vertex);
    return std::move(*stop_to_vertex.release());
}

void SerialTC::SetIdToStop(VertexId_Stop&& id_to_stop_) {
    id_to_stop = std::move(std::make_unique<VertexId_Stop>(std::forward<VertexId_Stop>(id_to_stop_)));
}

VertexId_Stop&& SerialTC::GetIdToStop() {
    assert(id_to_stop);
    return std::move(*id_to_stop.release());
}

void SerialTC::SetEdgeToBusSpan(Edge_BusSpan&& edge_to_bus_span_) {
    edge_to_bus_span = std::move(std::make_unique<Edge_BusSpan>(std::forward<Edge_BusSpan>(edge_to_bus_span_)));
}

Edge_BusSpan&& SerialTC::GetEdgeToBusSpan() {
    assert(edge_to_bus_span);
    return std::move(*edge_to_bus_span.release());
}

void SerialTC::SerializeCatalogue(const Catalogue::TransportCatalogue& catalogue, 
                                                proto_tc::TransportCatalogue& pb_catalogue) {
 
    // resize fields pb_catalogue
    for(int i = 0; i < catalogue.GetStopCount(); ++i) {
        pb_catalogue.add_stops();
    }

    for(int i = 0; i < catalogue.GetBuses().size(); ++i) {
        pb_catalogue.add_buses();
    }

    for(int i = 0; i < catalogue.GetDistances().size(); ++i) {
        pb_catalogue.add_distances();
    }

    // fill all stops
    for(const auto& stop : catalogue.GetStops()) {
        proto_tc::Stop pb_stop;
        pb_stop.set_id(stop.id);
        pb_stop.set_name(stop.name_);
        pb_stop.set_lat(stop.coordinates.lat);
        pb_stop.set_lng(stop.coordinates.lng);

        *pb_catalogue.mutable_stops(stop.id) = std::move(pb_stop); 
    }

    // fill add buses
    size_t idx = 0;
    for(const auto& bus : catalogue.GetBuses()) {
        
        proto_tc::Bus pb_bus;
        pb_bus.set_name(bus.name_);

        for(const auto& stop : bus.route_) {
            pb_bus.add_route(stop->id);
        }
        pb_bus.set_unique_size(bus.unique_size);
        pb_bus.set_length_(bus.length_);
        pb_bus.set_geo_length(bus.geo_length_);
        pb_bus.set_is_roundtrip(bus.is_roundtrip_);
        pb_bus.set_last_stop(bus.last_stop_->id);

        *pb_catalogue.mutable_buses(idx++) = std::move(pb_bus);
    }

    // fill all distances
    idx = 0;
    for(const auto& [stops, dist] : catalogue.GetDistances()) {
        proto_tc::Distance pb_dist;
        pb_dist.set_stop_x(stops.first->id);
        pb_dist.set_stop_y(stops.second->id);
        pb_dist.set_distance(dist);

        *pb_catalogue.mutable_distances(idx++) = std::move(pb_dist);
    }
}


void SerialTC::SerializeRenderSettings(proto_map_render::RenderSettings& pb_render_settings) {
    const auto& rs = *render_settings;

    pb_render_settings.set_width(rs.width);
    pb_render_settings.set_height(rs.height);
    pb_render_settings.set_padding(rs.padding);
    pb_render_settings.set_line_wigth(rs.line_wigth);
    pb_render_settings.set_stop_radius(rs.stop_radius);
    pb_render_settings.set_bus_label_font_size(rs.bus_label_font_size);
    pb_render_settings.set_bus_label_offset_first(rs.bus_label_offset.first);
    pb_render_settings.set_bus_label_offset_second(rs.bus_label_offset.second);
    pb_render_settings.set_stop_label_font_size(rs.stop_label_font_size);
    pb_render_settings.set_stop_label_offset_first(rs.stop_label_offset.first);
    pb_render_settings.set_stop_label_offset_second(rs.stop_label_offset.second);

    if(std::holds_alternative<std::string>(rs.underlayer_color)) {
        pb_render_settings.mutable_underlayer_color()->set_str(std::get<std::string>(rs.underlayer_color));
    } else if(std::holds_alternative<svg::Rgb>(rs.underlayer_color)) {
        const auto& rgb = std::get<svg::Rgb>(rs.underlayer_color);
        proto_svg::Rgb pb_rgb;
        pb_rgb.set_red(rgb.red);
        pb_rgb.set_green(rgb.green);
        pb_rgb.set_blue(rgb.blue);
        *pb_render_settings.mutable_underlayer_color()->mutable_rgb() = std::move(pb_rgb);
    } else if(std::holds_alternative<svg::Rgba>(rs.underlayer_color)) {
        const auto& rgba = std::get<svg::Rgba>(rs.underlayer_color);
        proto_svg::Rgba pb_rgba;
        pb_rgba.set_red(rgba.red);
        pb_rgba.set_green(rgba.green);
        pb_rgba.set_blue(rgba.blue);
        pb_rgba.set_opacity(rgba.opacity);
        *pb_render_settings.mutable_underlayer_color()->mutable_rgba() = std::move(pb_rgba);
    }

    pb_render_settings.set_underlayer_width(rs.underlayer_width);

    size_t idx = 0;
    for(const auto& color : rs.color_palette) {
        proto_svg::Color pb_color;
        if(std::holds_alternative<std::string>(color)) {
            pb_color.set_str(std::get<std::string>(color));
        } else if(std::holds_alternative<svg::Rgb>(color)) {
            const auto& rgb = std::get<svg::Rgb>(color);
            proto_svg::Rgb pb_rgb;
            pb_rgb.set_red(rgb.red);
            pb_rgb.set_green(rgb.green);
            pb_rgb.set_blue(rgb.blue);
            *pb_color.mutable_rgb() = std::move(pb_rgb);
        } else if(std::holds_alternative<svg::Rgba>(color)) {
            const auto& rgba = std::get<svg::Rgba>(color);
            proto_svg::Rgba pb_rgba;
            pb_rgba.set_red(rgba.red);
            pb_rgba.set_green(rgba.green);
            pb_rgba.set_blue(rgba.blue);
            pb_rgba.set_opacity(rgba.opacity);
            *pb_color.mutable_rgba() = std::move(pb_rgba);
        }
        pb_render_settings.add_color_palette();
        *pb_render_settings.mutable_color_palette(idx++) = std::move(pb_color); 
    }
}

void SerialTC::SerializeRouter(proto_tr::TransportRouter& pb_router) {

    proto_tr::RoutingSettings pb_routing_settings;
    pb_routing_settings.set_bus_velocity((*routing_settings).bus_velocity_);
    pb_routing_settings.set_bus_wait_time((*routing_settings).bus_wait_time_);
    *pb_router.mutable_routing_setting() = std::move(pb_routing_settings);

    size_t edges_count = (*graph_ptr).GetEdgeCount();
    for(auto idx = 0; idx < edges_count; ++idx) {
        proto_graph::Edge pb_edge;
        const auto& edge = (*graph_ptr).GetEdge(idx);
        pb_edge.set_from(edge.from);
        pb_edge.set_to(edge.to);
        pb_edge.set_weight(edge.weight);
        pb_router.add_graph();
        *pb_router.mutable_graph(idx) = std::move(pb_edge);
    }

    size_t idx = 0;
    for(const auto& [key, value] : *(stop_to_vertex)) {
        proto_tr::Stop_VertexId pb_stop_vertex;
        pb_stop_vertex.set_stop_id(key->id);
        pb_stop_vertex.set_vertex_id(value);
        pb_router.add_stop_vertex();
        *pb_router.mutable_stop_vertex(idx++) = std::move(pb_stop_vertex);
    }

    idx = 0;
    for(const auto& [key, value] : *(edge_to_bus_span)) {
        proto_tr::Edge_BusSpan pb_edge_to_bus_span;
        pb_edge_to_bus_span.set_vertex_from(key.first);
        pb_edge_to_bus_span.set_vetex_to(key.second);
        pb_edge_to_bus_span.set_bus_name(value.first->name_);
        pb_edge_to_bus_span.set_span_count(value.second);

        pb_router.add_edge_bus_span();
        *pb_router.mutable_edge_bus_span(idx++) = std::move(pb_edge_to_bus_span);
    }
}

bool SerialTC::Serialization(const Catalogue::TransportCatalogue& catalogue) {
    proto_tc::GENERAL_DATA general_data;

    proto_tc::TransportCatalogue pb_catalogue;
    SerializeCatalogue(catalogue, pb_catalogue);
    *general_data.mutable_tc() = std::move(pb_catalogue);

    proto_map_render::RenderSettings pb_render_settings;
    SerializeRenderSettings(pb_render_settings);
    *general_data.mutable_renderer_settings() = std::move(pb_render_settings);

    proto_tr::TransportRouter pb_router;
    SerializeRouter(pb_router);
    *general_data.mutable_tr() = std::move(pb_router);

    // serialize 
    std::ofstream ofs(std::filesystem::path(GetSerializationSettings().file), std::ios::binary);
    return general_data.SerializeToOstream(&ofs);
}

void SerialTC::DeserializeCatalogue(const proto_tc::TransportCatalogue& pb_catalogue,
                                            Catalogue::TransportCatalogue& catalogue) {

    for(const auto& pb_stop : pb_catalogue.stops()) {
        catalogue.AddStop(pb_stop.id(), pb_stop.name(), pb_stop.lat(), pb_stop.lng());
    }

    for(const auto& bus: pb_catalogue.buses()) {
        
        std::vector<const domain::Stop*> route;
        route.reserve(bus.route().size());

        for(const auto& stop_id : bus.route()) {
           route.push_back(catalogue.FindStop(pb_catalogue.stops(stop_id).name()));
        }

        catalogue.AddBus(domain::Bus{ bus.name(), std::move(route), bus.unique_size(), bus.length_(), bus.geo_length(), 
            bus.is_roundtrip(), catalogue.FindStop(pb_catalogue.stops(bus.last_stop()).name())});
    }

    for(const auto& distance : pb_catalogue.distances()) {
        catalogue.AddDistance(pb_catalogue.stops(distance.stop_x()).name(), 
            pb_catalogue.stops(distance.stop_y()).name(), distance.distance());
    }
}

void SerialTC::DeserializeRenderSettings(const proto_map_render::RenderSettings& pb_render_set,
                        renderer::RenderSettings& rs) {

    rs.width = pb_render_set.width();
    rs.height = pb_render_set.height();
    rs.padding = pb_render_set.padding();
    rs.line_wigth = pb_render_set.line_wigth();
    rs.stop_radius = pb_render_set.stop_radius();
    rs.bus_label_font_size = pb_render_set.bus_label_font_size();
    rs.bus_label_offset = 
            {pb_render_set.bus_label_offset_first(), pb_render_set.bus_label_offset_second() };
    rs.stop_label_font_size = pb_render_set.stop_label_font_size();
    rs.stop_label_offset = 
            {pb_render_set.stop_label_offset_first(), pb_render_set.stop_label_offset_second() };

    if(pb_render_set.underlayer_color().color_case() == proto_svg::Color::kStr) {
        rs.underlayer_color = svg::Color(pb_render_set.underlayer_color().str());
    } else if(pb_render_set.underlayer_color().color_case() == proto_svg::Color::kRgb) {
        svg::Rgb rgb;
        proto_svg::Rgb pb_rgb = pb_render_set.underlayer_color().rgb();
        rgb.red = pb_rgb.red();
        rgb.green = pb_rgb.green();
        rgb.blue = pb_rgb.blue();
        rs.underlayer_color = svg::Color(std::move(rgb));
    } else if(pb_render_set.underlayer_color().color_case() == proto_svg::Color::kRgba) {
        svg::Rgba rgba;
        proto_svg::Rgba pb_rgba = pb_render_set.underlayer_color().rgba();
        rgba.red = pb_rgba.red();
        rgba.green = pb_rgba.green();
        rgba.blue = pb_rgba.blue();
        rgba.opacity = pb_rgba.opacity();
        rs.underlayer_color = svg::Color(std::move(rgba));
    }
    else {
        rs.underlayer_color = svg::NoneColor;
    }
    rs.underlayer_width = pb_render_set.underlayer_width();

    for(const auto& pb_color : pb_render_set.color_palette()) {
        if(pb_color.color_case() == proto_svg::Color::kStr) {
            rs.color_palette.push_back(svg::Color(pb_color.str()));
        } else if(pb_color.color_case() == proto_svg::Color::kRgb) {
            svg::Rgb rgb;
            proto_svg::Rgb pb_rgb = pb_color.rgb();
            rgb.red = pb_rgb.red();
            rgb.green = pb_rgb.green();
            rgb.blue = pb_rgb.blue();
            rs.color_palette.push_back(svg::Color(std::move(rgb)));
        } else if(pb_color.color_case() == proto_svg::Color::kRgba) {
            svg::Rgba rgba;
            proto_svg::Rgba pb_rgba = pb_color.rgba();
            rgba.red = pb_rgba.red();
            rgba.green = pb_rgba.green();
            rgba.blue = pb_rgba.blue();
            rgba.opacity = pb_rgba.opacity();
            rs.color_palette.push_back(svg::Color(std::move(rgba)));
        }
        else {
            rs.color_palette.push_back(svg::NoneColor);
        }
    }
}

void SerialTC::DeserializeTransportRouter(const proto_tr::TransportRouter& pb_router, const Catalogue::TransportCatalogue& catalogue) {
    
    TRouter::RoutingSettings local_rs;
    proto_tr::RoutingSettings pb_routing_settings = pb_router.routing_setting();
    local_rs.bus_velocity_ = pb_routing_settings.bus_velocity();
    local_rs.bus_wait_time_ = pb_routing_settings.bus_wait_time();
    SetRoutingSettings(std::move(local_rs));
    
    graph graph_(catalogue.GetStopCount() * 2);
    for(const auto& pb_edge : pb_router.graph()) {
        graph_.AddEdge(Edge<double>{static_cast<size_t>(pb_edge.from()), 
                    static_cast<size_t>(pb_edge.to()), pb_edge.weight()});
    }
    SetGraph(std::move(graph_));
    
    Stop_VertexId stop_to_vertex;
    VertexId_Stop id_to_stop;
    for(const auto& pb_stop_to_vertex : pb_router.stop_vertex()) {
        const auto& stop_id = pb_stop_to_vertex.stop_id();
        const auto& vertex_id = pb_stop_to_vertex.vertex_id();

        stop_to_vertex[catalogue.FindStop(stop_id)] = vertex_id;
        id_to_stop[vertex_id] = catalogue.FindStop(stop_id);
    }
    SetStopToVertex(std::move(stop_to_vertex));
    SetIdToStop(std::move(id_to_stop));
    Edge_BusSpan edge_to_bus_span;
    for(const auto& pb_edge_to_bus_span : pb_router.edge_bus_span()) {
        edge_to_bus_span[{pb_edge_to_bus_span.vertex_from(), pb_edge_to_bus_span.vetex_to()}] = 
            {catalogue.FindBus(pb_edge_to_bus_span.bus_name()), pb_edge_to_bus_span.span_count()};
    }
    SetEdgeToBusSpan(std::move(edge_to_bus_span));
}

bool SerialTC::Deserialization(Catalogue::TransportCatalogue& catalogue) {

    proto_tc::GENERAL_DATA general_data;
    std::ifstream ifs(std::filesystem::path(GetSerializationSettings().file), std::ios::binary);

    if(!general_data.ParseFromIstream(&ifs)) {
        return false;
    }

    DeserializeCatalogue(general_data.tc(), catalogue);
    renderer::RenderSettings rs;
    DeserializeRenderSettings(general_data.renderer_settings(), rs);
    SetRenderSettings(std::move(rs));
    DeserializeTransportRouter(general_data.tr(), catalogue);

    return true;
}

} // namespace serial