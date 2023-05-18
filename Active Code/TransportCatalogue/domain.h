#pragma once

#include "geo.h"
#include "svg.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

namespace domain {

struct Stop {
    std::string name_; // название остановки
    geo::Coordinates coordinates; // координаты остановки
    explicit Stop(std::string name, double latit, double longt)
        : name_(name)
        , coordinates({latit, longt})
    {
    }
};

struct Bus {
    std::string name_; // название маршрута
    std::vector<const Stop*> route_; // маршрут по остановкам
    size_t unique_size = 0;
    int64_t length_ = 0;
    double geo_length_ = 0;
    explicit Bus(std::string name, const std::vector<const Stop*>& route, size_t size, int64_t length, double geo_length)
        : name_(name)
        , route_(route)
        , unique_size(size)
        , length_(length)
        , geo_length_(geo_length)
    {
    }
    bool CheckStop(const Stop* stop) const;
};

struct BusInfo
{
    bool check = false;
    size_t size = 0;
    size_t unique_size = 0;
    int64_t length = 0;
    double geo_length = .0;
};

struct HacherPair
{
    size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const; // catalogue dist_btn_stops_
    size_t operator()(const std::pair<std::string_view, std::string_view>& stops) const; // json reader : stops_to_dstns
};


inline const double EPSILON = 1e-6;
bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}

class SphereProjector {
public:
    // points_begin и points_end задают начало и конец интервала элементов geo::Coordinates
    template <typename PointInputIt>
    SphereProjector(PointInputIt points_begin, PointInputIt points_end,
        double max_width, double max_height, double padding);

    // Проецирует широту и долготу в координаты внутри SVG-изображения
    svg::Point operator()(geo::Coordinates coords) const;

private:
    double padding_;
    double min_lon_ = 0;
    double max_lat_ = 0;
    double zoom_coeff_ = 0;
};

template <typename PointInputIt>
SphereProjector::SphereProjector(PointInputIt points_begin, PointInputIt points_end,
            double max_width, double max_height, double padding)
    : padding_(padding) //
{
    // Если точки поверхности сферы не заданы, вычислять нечего
    if (points_begin == points_end) {
        return;
    }

    // Находим точки с минимальной и максимальной долготой
    const auto [left_it, right_it] = std::minmax_element(
        points_begin, points_end,
        [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
    min_lon_ = left_it->lng;
    const double max_lon = right_it->lng;

    // Находим точки с минимальной и максимальной широтой
    const auto [bottom_it, top_it] = std::minmax_element(
        points_begin, points_end,
        [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
    const double min_lat = bottom_it->lat;
    max_lat_ = top_it->lat;

    // Вычисляем коэффициент масштабирования вдоль координаты x
    std::optional<double> width_zoom;
    if (!IsZero(max_lon - min_lon_)) {
        width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
    }

    // Вычисляем коэффициент масштабирования вдоль координаты y
    std::optional<double> height_zoom;
    if (!IsZero(max_lat_ - min_lat)) {
        height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
    }

    if (width_zoom && height_zoom) {
        // Коэффициенты масштабирования по ширине и высоте ненулевые,
        // берём минимальный из них
        zoom_coeff_ = std::min(*width_zoom, *height_zoom);
    } else if (width_zoom) {
        // Коэффициент масштабирования по ширине ненулевой, используем его
        zoom_coeff_ = *width_zoom;
    } else if (height_zoom) {
        // Коэффициент масштабирования по высоте ненулевой, используем его
        zoom_coeff_ = *height_zoom;
    }
}

}