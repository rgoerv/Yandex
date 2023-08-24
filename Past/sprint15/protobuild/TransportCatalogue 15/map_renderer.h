#pragma once

#include "svg.h"
#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"
#include "unordered_set"

namespace renderer {

struct RenderSettings {
    RenderSettings() = default;
    RenderSettings(const json::Document& render_settings_) noexcept;

    std::vector<svg::Color> GetPalette(const json::Array& palette_settings) const;
    svg::Color GetColor(const json::Node& color) const;

    double width;
    double height;
    double padding;
    double line_wigth;
    double stop_radius;
    int bus_label_font_size;
    std::pair<double, double> bus_label_offset;
    int stop_label_font_size;
    std::pair<double, double> stop_label_offset;
    svg::Color underlayer_color;
    double underlayer_width;
    std::vector<svg::Color> color_palette;
};

class MapRenderer {
public:
    MapRenderer(RenderSettings settings, const Catalogue::TransportCatalogue& ts);
    void Render(std::ostream& out) const;
private:
    RenderSettings render_settings;
    svg::Document map_render;
    const Catalogue::TransportCatalogue& catalogue;
    std::set<std::string_view> sort_buses;
    std::set<std::string_view> sort_stops;
    std::unique_ptr<const domain::SphereProjector> proj = nullptr;

    void BuildMap();
    std::unordered_set<geo::Coordinates, domain::HacherPair> GetCoordinates() const;
    void NextPos(int& palette_pos) const;
    void AddBusLines();
    void AddBusNames();
    void AddCircleStop();
    void AddStopsNames();
};

}

