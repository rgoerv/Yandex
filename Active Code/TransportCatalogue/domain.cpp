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

svg::Point SphereProjector::operator()(geo::Coordinates coords) const {
    return {
        (coords.lng - min_lon_) * zoom_coeff_ + padding_,
        (max_lat_ - coords.lat) * zoom_coeff_ + padding_
    };
}

}