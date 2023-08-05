#include "map_renderer.h"
#include "domain.h"
#include "svg.h"

#include <vector>
#include <iostream>

namespace renderer {

using namespace std::literals;

MapRenderer::MapRenderer(json::Document settings, const Catalogue::TransportCatalogue& ts)
    : render_settings(std::move(settings))
    , catalogue(ts)
{
    const auto& geo_coords = GetCoordinates();
    proj = std::make_unique<const domain::SphereProjector>(domain::SphereProjector{
        geo_coords.begin(), geo_coords.end(), render_settings.width, render_settings.height, render_settings.padding
    });

    for (const auto& [busname, bus] : catalogue.GetBusNameToBus()) {
        sort_buses.insert(busname);
    }

    BuildMap();
}

RenderSettings::RenderSettings(const json::Document& render_settings_) noexcept 
    : render_settings(render_settings_)
{
    const auto& settings = render_settings.GetRoot().AsDict();

    width = settings.at("width"s).AsDouble();
    height = settings.at("height"s).AsDouble();
    padding = settings.at("padding"s).AsDouble();
    line_wigth = settings.at("line_width"s).AsDouble();
    stop_radius = settings.at("stop_radius"s).AsDouble();
    bus_label_font_size = settings.at("bus_label_font_size"s).AsInt();

    const auto& bus_offset_arr = settings.at("bus_label_offset"s).AsArray();
    bus_label_offset = std::make_pair(bus_offset_arr.at(0).AsDouble(),
                        bus_offset_arr.at(1).AsDouble());

    stop_label_font_size = settings.at("stop_label_font_size"s).AsInt();

    const auto& stop_offset_arr = settings.at("stop_label_offset"s).AsArray();
    stop_label_offset = std::make_pair(stop_offset_arr.at(0).AsDouble(),
                        stop_offset_arr.at(1).AsDouble());

    underlayer_color = GetColor(settings.at("underlayer_color"s));
    underlayer_width = settings.at("underlayer_width"s).AsDouble();
    color_palette = GetPalette();
}

void MapRenderer::Render(std::ostream& out) const {
    map_render.Render(out);
}

void MapRenderer::NextPos(int& palette_pos) const {
    if (palette_pos + 1 >= static_cast<int>(render_settings.color_palette.size())) {
        palette_pos = 0;
        return;
    }
    ++palette_pos;
}

void MapRenderer::BuildMap() {

    for (const auto& busname : sort_buses) {
        const auto& route = catalogue.FindBus(busname)->route_;
        for (const auto& stop : route) {
            sort_stops.insert(stop->name_);
        }
    }

    AddBusLines();
    AddBusNames();
    AddCircleStop();
    AddStopsNames();
}

svg::Color RenderSettings::GetColor(const json::Node& color) const {
    svg::Color result_color = svg::NoneColor;
    if (color.IsString()) {
        result_color = svg::Color { color.AsString() };
    } else if (color.IsArray()) {
        const auto& arr = color.AsArray();
        if (arr.size() == 3) {
            result_color = svg::Color { svg::Rgb { 
                static_cast<uint8_t>(arr[0].AsInt()),
                static_cast<uint8_t>(arr[1].AsInt()),
                static_cast<uint8_t>(arr[2].AsInt()) } };
        } else if (arr.size() == 4) {
            result_color = svg::Color { svg::Rgba { 
                static_cast<uint8_t>(arr[0].AsInt()),
                static_cast<uint8_t>(arr[1].AsInt()),
                static_cast<uint8_t>(arr[2].AsInt()),
                arr[3].AsDouble() } };
        }
    }
    return result_color;
}

std::vector<svg::Color> RenderSettings::GetPalette() const {
    const auto& palette_settings = render_settings.GetRoot().AsDict().at("color_palette").AsArray();
    std::vector<svg::Color> palette;
    palette.reserve(palette_settings.size());

    for (const auto& color : palette_settings) {
        palette.push_back(GetColor(color));
    }
    return palette;
}

std::unordered_set<geo::Coordinates, domain::HacherPair> MapRenderer::GetCoordinates() const {
    const auto& busname_to_bus_ = catalogue.GetBusNameToBus();
    std::unordered_set<geo::Coordinates, domain::HacherPair> geo_coords;
    geo_coords.reserve(busname_to_bus_.size());
    for (const auto& [busname, bus] : busname_to_bus_) {
        for (const auto& stop : catalogue.FindBus(busname)->route_) {
            geo_coords.insert(stop->coordinates); 
        }
    }
    return geo_coords;
}

void MapRenderer::AddBusLines()  {

    const auto& palette = render_settings.color_palette;
    int palette_pos = 0;

    for (const auto& busname : sort_buses) {
        svg::Polyline busline_map;
        const auto& route = catalogue.FindBus(busname)->route_;

        for (const auto& stop : route) {
            busline_map.AddPoint((*proj)(stop->coordinates));
        }
        busline_map
            .SetStrokeColor(palette[palette_pos])
            .SetStrokeWidth(render_settings.line_wigth)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
            .SetFillColor(svg::NoneColor);
        map_render.Add(busline_map);

        if (!route.empty()) {
            //
            NextPos(palette_pos);
        }
    }
}

void MapRenderer::AddBusNames() {
    const auto& palette = render_settings.color_palette;
    int palette_pos = 0;

    for (const auto& busname : sort_buses) {
        const auto& bus = catalogue.FindBus(busname);
        const auto& route = bus->route_;

        if (!route.empty()) {
            svg::Text busname_map1_underlayer;

            busname_map1_underlayer
                .SetData(static_cast<std::string>(busname))
                .SetPosition({ (*proj)(route.at(0)->coordinates) })
                .SetOffset(svg::Point { render_settings.bus_label_offset.first,
                    render_settings.bus_label_offset.second })
                .SetFontSize(render_settings.bus_label_font_size)
                .SetFontFamily("Verdana"s)
                .SetFontWeight("bold"s);

            svg::Text busname_map1(busname_map1_underlayer);

            busname_map1_underlayer
                .SetFillColor(render_settings.underlayer_color)
                .SetStrokeColor(render_settings.underlayer_color)
                .SetStrokeWidth(render_settings.underlayer_width)
                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

            busname_map1
                .SetFillColor(palette[palette_pos]);

            map_render.Add(busname_map1_underlayer);
            map_render.Add(busname_map1);

            if (route.front() != bus->last_stop_) {
                svg::Text busname_map2_underlayer;

                busname_map2_underlayer
                    .SetData(static_cast<std::string>(busname))
                    .SetPosition({ (*proj)(catalogue.FindBus(busname)->last_stop_->coordinates) })
                    .SetOffset(svg::Point { render_settings.bus_label_offset.first,
                       render_settings.bus_label_offset.second })
                    .SetFontSize(render_settings.bus_label_font_size)
                    .SetFontFamily("Verdana"s)
                    .SetFontWeight("bold"s);

                svg::Text busname_map2(busname_map2_underlayer);

                busname_map2_underlayer
                    .SetFillColor(render_settings.underlayer_color)
                    .SetStrokeColor(render_settings.underlayer_color)
                    .SetStrokeWidth(render_settings.underlayer_width)
                    .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                    .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

                busname_map2
                    .SetFillColor(palette[palette_pos]);

                map_render.Add(busname_map2_underlayer);
                map_render.Add(busname_map2);
            }
            // 
            NextPos(palette_pos);
        }
    }
}

void MapRenderer::AddCircleStop() {

    for (const auto& stop : sort_stops) {
        svg::Circle buscircle_map;
        buscircle_map
            .SetCenter((*proj)(catalogue.FindStop(stop)->coordinates))
            .SetRadius(render_settings.stop_radius)
            .SetFillColor("white"s);
        map_render.Add(buscircle_map);
    }           
}

void MapRenderer::AddStopsNames() {

    for (const auto& stop : sort_stops) {
        svg::Text stopname_map_underlayer;

        stopname_map_underlayer
            .SetPosition((*proj)(catalogue.FindStop(stop)->coordinates))
            .SetOffset(svg::Point { render_settings.stop_label_offset.first,
                render_settings.stop_label_offset.second })
            .SetFontSize(render_settings.stop_label_font_size)
            .SetFontFamily("Verdana")
            .SetData(catalogue.FindStop(stop)->name_);

        svg::Text stopname_map(stopname_map_underlayer);
        
        stopname_map_underlayer
            .SetFillColor(render_settings.underlayer_color)
            .SetStrokeColor(render_settings.underlayer_color)
            .SetStrokeWidth(render_settings.underlayer_width)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);

        stopname_map.SetFillColor("black"s);

        map_render.Add(stopname_map_underlayer);
        map_render.Add(stopname_map);
    }
}

} // namespace renderer