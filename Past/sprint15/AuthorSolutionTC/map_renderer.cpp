#include "map_renderer.h"

#include "transport_catalogue.h"
#include <algorithm>
#include <unordered_set>

namespace transport_catalogue::renderer {

using namespace std::literals;

namespace {

constexpr double EPSILON = 1e-6;
bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}

class SphereProjector {
public:
    template <typename PointInputIt>
    SphereProjector(PointInputIt points_begin, PointInputIt points_end, double max_width,
                    double max_height, double padding)
        : padding_(padding) {
        if (points_begin == points_end) {
            return;
        }

        const auto [left_it, right_it]
            = std::minmax_element(points_begin, points_end, [](auto lhs, auto rhs) {
                  return lhs.lng < rhs.lng;
              });
        min_lon_ = left_it->lng;
        const double max_lon = right_it->lng;

        const auto [bottom_it, top_it]
            = std::minmax_element(points_begin, points_end, [](auto lhs, auto rhs) {
                  return lhs.lat < rhs.lat;
              });
        const double min_lat = bottom_it->lat;
        max_lat_ = top_it->lat;

        std::optional<double> width_zoom;
        if (!IsZero(max_lon - min_lon_)) {
            width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
        }

        std::optional<double> height_zoom;
        if (!IsZero(max_lat_ - min_lat)) {
            height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
        }

        if (width_zoom && height_zoom) {
            zoom_coeff_ = std::min(*width_zoom, *height_zoom);
        } else if (width_zoom) {
            zoom_coeff_ = *width_zoom;
        } else if (height_zoom) {
            zoom_coeff_ = *height_zoom;
        }
    }

    svg::Point operator()(geo::Coordinates coords) const {
        return {(coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_};
    }

private:
    double padding_;
    double min_lon_ = 0;
    double max_lat_ = 0;
    double zoom_coeff_ = 0;
};

}  // namespace

std::vector<BusPtr> MapRenderer::MakeBuses(const TransportCatalogue& db) {
    const auto& bus_pool = db.GetBuses();

    std::vector<BusPtr> buses;
    buses.reserve(bus_pool.size());
    for (const Bus& bus : bus_pool) {
        buses.push_back(&bus);
    }
    std::sort(buses.begin(), buses.end(), [](BusPtr lhs, BusPtr rhs) {
        return lhs->name < rhs->name;
    });

    return buses;
}

MapRenderer::MapRenderer(RenderSettings render_settings, const TransportCatalogue& db)
    : render_settings_(std::move(render_settings))
    , buses_(MakeBuses(db))  //
{
    std::unordered_set<StopPtr> stops;
    for (const BusPtr bus : buses_) {
        stops.insert(bus->stops.begin(), bus->stops.end());
    }

    std::vector<geo::Coordinates> geo_coords;
    geo_coords.reserve(stops.size());
    for (const StopPtr stop : stops) {
        geo_coords.push_back(stop->position);
    }

    SphereProjector projector{geo_coords.begin(), geo_coords.end(), render_settings_.max_width,
                              render_settings_.max_height, render_settings_.padding};
    for (const StopPtr stop : stops) {
        stops_coords_[stop] = projector(stop->position);
    }
}

void MapRenderer::Draw(svg::ObjectContainer& container) const {
    RenderBusLines(container);
    RenderBusLabels(container);
    RenderStopPoints(container);
    RenderStopLabels(container);
}

void MapRenderer::RenderBusLines(svg::ObjectContainer& container) const {
    using namespace svg;
    size_t bus_index = 0;
    for (const BusPtr bus : buses_) {
        if (const auto& stops = bus->stops; !stops.empty()) {
            Polyline line;
            line.SetStrokeColor(GetBusLineColor(bus_index++))
                .SetStrokeWidth(render_settings_.line_width)
                .SetStrokeLineCap(StrokeLineCap::ROUND)
                .SetFillColor(NoneColor)
                .SetStrokeLineJoin(StrokeLineJoin::ROUND);

            for (const StopPtr stop : stops) {
                line.AddPoint(stops_coords_.at(stop));
            }
            container.Add(std::move(line));
        }
    }
}

void MapRenderer::RenderBusLabels(svg::ObjectContainer& container) const {
    using namespace svg;

    size_t bus_index = 0;
    for (const BusPtr bus : buses_) {
        if (bus->stops.empty()) {
            continue;
        }
        const auto& bus_color = GetBusLineColor(bus_index++);

        for (const StopPtr endpoint : bus->endpoints) {
            const auto& stop_coord = stops_coords_.at(endpoint);

            const auto base_text =  //
                Text()
                    .SetPosition(stop_coord)
                    .SetOffset(render_settings_.bus_label_offset)
                    .SetFontSize(render_settings_.bus_label_font_size)
                    .SetFontFamily(render_settings_.bus_label_font_family)
                    .SetFontWeight("bold"s)
                    .SetData(bus->name);
            container.Add(  //
                Text{base_text}
                    .SetFillColor(render_settings_.underlayer_color)
                    .SetStrokeColor(render_settings_.underlayer_color)
                    .SetStrokeWidth(render_settings_.underlayer_width)
                    .SetStrokeLineCap(StrokeLineCap::ROUND)
                    .SetStrokeLineJoin(StrokeLineJoin::ROUND));
            container.Add(Text{base_text}.SetFillColor(bus_color));
        }
    }
}

void MapRenderer::RenderStopPoints(svg::ObjectContainer& container) const {
    using namespace svg;
    for (const auto& [_, stop_coord] : stops_coords_) {
        container.Add(  //
            Circle()
                .SetRadius(render_settings_.stop_radius)
                .SetCenter(stop_coord)
                .SetFillColor("white"s));
    }
}

void MapRenderer::RenderStopLabels(svg::ObjectContainer& container) const {
    using namespace svg;
    for (const auto& [stop, stop_coord] : stops_coords_) {
        Text base_text;
        base_text  //
            .SetPosition(stop_coord)
            .SetOffset(render_settings_.stop_label_offset)
            .SetFontSize(render_settings_.stop_label_font_size)
            .SetFontFamily(render_settings_.stop_label_font_family)
            .SetData(stop->name);
        container.Add(  //
            Text{base_text}
                .SetFillColor(render_settings_.underlayer_color)
                .SetStrokeColor(render_settings_.underlayer_color)
                .SetStrokeWidth(render_settings_.underlayer_width)
                .SetStrokeLineCap(StrokeLineCap::ROUND)
                .SetStrokeLineJoin(StrokeLineJoin::ROUND));
        container.Add(Text{base_text}.SetFillColor(render_settings_.stop_label_color));
    }
}

const svg::Color& MapRenderer::GetBusLineColor(size_t index) const {
    static const svg::Color DEFAULT_COLOR = svg::Color{"black"s};

    const auto& palette = render_settings_.palette;

    return !palette.empty() ? palette[index % palette.size()] : DEFAULT_COLOR;
}

}  // namespace transport_catalogue::renderer