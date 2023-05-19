#pragma once

#include "svg.h"
#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"

namespace renderer {

struct RenderSettings {
    RenderSettings(const json::Document& render_settings);

    std::vector<svg::Color> GetPalette() const;

    json::Document render_settings;

    double width;
    double height;
    double padding;
    double line_wigth;
    double stop_radius;
    int bus_label_font_size;
    std::pair<double, double> bus_label_offset;
    int stop_label_font_size;
    std::pair<double, double> stop_label_offset;
    std::vector<svg::Color> underlayer_color;
    double underlayer_width;
    std::vector<svg::Color> color_palette;
};

class MapRenderer {
public:
    MapRenderer(json::Document settings, const Catalogue::TransportCatalogue& ts);

    void Render(std::ostream& out) const;

private:
    RenderSettings render_settings;
    svg::Document map_render;
    const Catalogue::TransportCatalogue& catalogue;


    void BuildMap();
    std::vector<svg::Color> GetPalette() const;
    std::vector<geo::Coordinates> GetCoordinates() const;

};

}

