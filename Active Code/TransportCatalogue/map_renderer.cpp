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

void MapRenderer::Render(std::ostream& out) const {
    map_render.Render(out);
}

void MapRenderer::BuildMap() {

    const auto& settings = render_settings.GetRoot().AsMap();

    const double WIDTH = settings.at("width").AsDouble();
    const double HEIGHT = settings.at("height").AsDouble();
    const double PADDING = settings.at("padding").AsDouble();

    std::vector<svg::Color> palitre;

    for (const auto& color : settings.at("color_palette").AsArray()) {

    }

    for (const auto& [busname, bus] : catalogue.GetBusNameToBus()) {

    }
    
}

}