#include "map_renderer.h"
#include "domain.h"
#include "svg.h"

#include <vector>
#include <iostream>

namespace renderer {

MapRenderer::MapRenderer(json::Document settings, const Catalogue::TransportCatalogue& ts)
    : render_settings(std::move(settings))
    , catalogue(ts)
{
    BuildMap();
}

RenderSettings::RenderSettings(const json::Document& render_settings_)
    : render_settings(render_settings_)
{
    const auto& settings = render_settings.GetRoot().AsMap();
    width = settings.at("width").AsDouble();
    height = settings.at("height").AsDouble();
    padding = settings.at("padding").AsDouble();
    color_palette = GetPalette();
    \


}

void MapRenderer::Render(std::ostream& out) const {
    map_render.Render(out);
}

void MapRenderer::BuildMap() {

    const auto& settings = render_settings.GetRoot().AsMap();


    const auto& palette = GetPalette();
    const auto& geo_coords = GetCoordinates();

    int palette_pos = 0;
    int palette_size = static_cast<int>(palette.size());

    auto next_pos = [&palette_size](int& palette_pos) {
        if (palette_pos + 1 >= palette_size) {
            palette_pos = 0;
        }
        ++palette_pos;
    };

    const domain::SphereProjector proj{
        geo_coords.begin(), geo_coords.end(), WIDTH, HEIGHT, PADDING
    };

    for (const auto& [busname, bus] : catalogue.GetBusNameToBus()) {
        svg::Polyline busline;
        const auto& route = catalogue.FindBus(busname)->route_;

        for (const auto& stop : route) {
            busline.AddPoint(proj(stop->coordinates));
        }

        
            busline
                .SetStrokeColor(palette[palette_pos])
                .SetStrokeWidth(settings.at("line_width").AsDouble())
                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                .SetFillColor(svg::NoneColor);
            map_render.Add(busline);
        if (!route.empty()) {
            next_pos(palette_pos);
        }
    }
    
}

std::vector<svg::Color> RenderSettings::GetPalette() const {
    const auto& palette_settings = render_settings.GetRoot().AsMap().at("color_palette").AsArray();
    std::vector<svg::Color> palette;
    palette.reserve(palette_settings.size());

    for (const auto& color : palette_settings) {
        if (color.IsString()) {
            palette.push_back(svg::Color { color.AsString() });
        } else if (color.IsArray()) {
            const auto& arr = color.AsArray();
            if (arr.size() == 3) {
                palette.push_back(svg::Color { svg::Rgb { static_cast<uint8_t>(arr[0].AsInt()),
                    static_cast<uint8_t>(arr[1].AsInt()),
                    static_cast<uint8_t>(arr[2].AsInt()) } });
            } else if (arr.size() == 4) {
                palette.push_back(svg::Color { svg::Rgba { static_cast<uint8_t>(arr[0].AsInt()),
                    static_cast<uint8_t>(arr[1].AsInt()),
                    static_cast<uint8_t>(arr[2].AsInt()),
                    arr[3].AsDouble() } });
            }
        }
    }
    return palette;
}

std::vector<geo::Coordinates> MapRenderer::GetCoordinates() const {
    const auto& busname_to_bus_ = catalogue.GetBusNameToBus();
    std::vector<geo::Coordinates> geo_coords;
    geo_coords.reserve(busname_to_bus_.size());
    for (const auto& [busname, bus] : busname_to_bus_) {
        for (const auto& stop : catalogue.FindBus(busname)->route_) {
            geo_coords.push_back(stop->coordinates); 
        }
    }
    return geo_coords;
}

} // namespace renderer