#include "domain.h"
#include "geo.h"

namespace domain {

bool Bus::CheckStop(const Stop* stop) const
{
    return std::count(route_.begin(), route_.end(), stop) > 0;
}

size_t HacherPair::operator()(const std::pair<const Stop*, const Stop*>& stops) const
{
    return std::hash<const void*> {}(stops.first)
            + std::hash<const void*> {}(stops.second) * 37;
}

size_t HacherPair::operator()(const std::pair<std::string_view, std::string_view>& stops) const {
    return std::hash<std::string_view> {}(stops.first)
        + std::hash<std::string_view> {}(stops.second) * 37;
}

size_t HacherPair::operator()(const geo::Coordinates& coords) const {
    return std::hash<double> {}(coords.lat)
        + std::hash<double> {}(coords.lng) * 37;
}

size_t HacherPair::operator()(const std::pair<const Bus*, size_t>& bus_span_count) const {
    return std::hash<const void*> {}(bus_span_count.first)
        + std::hash<size_t> {}(bus_span_count.second) * 37;
}

size_t HacherPair::operator()(const std::pair<size_t, size_t>& vertex_ids) const {
    return std::hash<size_t> {}(vertex_ids.first)
        + std::hash<size_t> {}(vertex_ids.second) * 37;
}

inline const double EPSILON = 1e-6;
bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}

svg::Point SphereProjector::operator()(geo::Coordinates coords) const {
    return {
        (coords.lng - min_lon_) * zoom_coeff_ + padding_,
        (max_lat_ - coords.lat) * zoom_coeff_ + padding_
    };
}

}