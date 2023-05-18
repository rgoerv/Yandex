#include "geo.h"

#include <cmath>

namespace geo {

bool Coordinates::operator==(const Coordinates& other) const
{
    return lat == other.lat && lng == other.lng;
}

bool Coordinates::operator!=(const Coordinates& other) const
{
    return !(*this == other);
}

double ComputeDistance(Coordinates from, Coordinates to) {
    using namespace std;
    if (from == to) {
        return 0;
    }
    return acos(sin(from.lat * DR) * sin(to.lat * DR)
                + cos(from.lat * DR) * cos(to.lat * DR) * cos(abs(from.lng - to.lng) * DR))
        * EARTH_RADIUS;
}

}  // namespace geo