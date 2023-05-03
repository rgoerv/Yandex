#pragma once

#include <cmath>

inline static const double PI = 3.1415926535;
inline static const double DR = PI / 180.;
inline static const int64_t EARTH_RADIUS = 6371000;

struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const
    {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coordinates& other) const
    {
        return !(*this == other);
    }
};

inline double ComputeDistance(Coordinates from, Coordinates to)
{
    using namespace std;
    if (from == to) {
        return 0;
    }
    return acos(sin(from.lat * DR) * sin(to.lat * DR)
               + cos(from.lat * DR) * cos(to.lat * DR) * cos(abs(from.lng - to.lng) * DR))
        * EARTH_RADIUS;
}