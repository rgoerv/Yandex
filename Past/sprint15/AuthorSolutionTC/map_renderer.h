#pragma once

#include "domain.h"
#include "svg.h"

#include <cassert>
#include <map>

namespace transport_catalogue {

class TransportCatalogue;

namespace renderer {

struct RenderSettings {
    std::vector<svg::Color> palette;

    double max_width = 1200;
    double max_height = 1200;
    double padding = 100;

    double line_width = 4.0;
    double stop_radius = 2;

    svg::Color underlayer_color{std::string("white")};
    double underlayer_width = 2;

    svg::Point stop_label_offset{3, -5};
    uint32_t stop_label_font_size = 12;
    svg::Color stop_label_color{std::string("black")};
    std::string stop_label_font_family{"Verdana"};

    svg::Point bus_label_offset{10, 10};
    uint32_t bus_label_font_size = 20;
    std::string bus_label_font_family{"Verdana"};
};

class MapRenderer : public svg::Drawable {
public:
    MapRenderer(RenderSettings render_settings, const TransportCatalogue& db);

    void Draw(svg::ObjectContainer& container) const override;

    const RenderSettings& GetRenderSettings() const {
        return render_settings_;
    }
    const auto& GetStopsCoords() const {
        return stops_coords_;
    }

private:
    void RenderBusLines(svg::ObjectContainer& container) const;
    void RenderBusLabels(svg::ObjectContainer& container) const;
    void RenderStopPoints(svg::ObjectContainer& container) const;
    void RenderStopLabels(svg::ObjectContainer& container) const;
    const svg::Color& GetBusLineColor(size_t index) const;

    MapRenderer() = default;
    static std::vector<BusPtr> MakeBuses(const TransportCatalogue& db);

    struct SortedByName {
        bool operator()(StopPtr lhs, StopPtr rhs) const {
            assert(lhs);
            assert(rhs);
            return lhs->name < rhs->name;
        }
    };
    RenderSettings render_settings_;
    std::vector<BusPtr> buses_;
    std::map<StopPtr, svg::Point, SortedByName> stops_coords_;

    friend class MapRendererBuilder;
};

class MapRendererBuilder {
public:
    MapRendererBuilder(RenderSettings render_settings, const TransportCatalogue& db) {
        data_.render_settings_ = std::move(render_settings);
        data_.buses_ = MapRenderer::MakeBuses(db);
    }

    void SetStopCoords(StopPtr stop_ptr, svg::Point coords) {
        data_.stops_coords_[stop_ptr] = coords;
    }

    MapRenderer Build() && {
        return std::move(data_);
    }
    
private:
    MapRenderer data_;
};

}  // namespace renderer

}  // namespace transport_catalogue