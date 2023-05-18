#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdint>

namespace geo {

inline static const double PI = 3.1415926535;
inline static const double DR = M_PI / 180.;
inline static const int64_t EARTH_RADIUS = 6371000;

struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const;
    bool operator!=(const Coordinates& other) const;
};

double ComputeDistance(Coordinates from, Coordinates to);
}