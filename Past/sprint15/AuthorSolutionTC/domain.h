#pragma once
#include "geo.h"

#include <string>
#include <vector>

namespace transport_catalogue {

struct Stop {
    Stop(std::string name, geo::Coordinates position)
        : name(std::move(name))
        , position(position) {
    }

    std::string name;
    geo::Coordinates position;
};
using StopPtr = const Stop*;

struct Bus {
    Bus(std::string name, std::vector<StopPtr> stops, std::vector<StopPtr> endpoints)
        : name(std::move(name))
        , stops(std::move(stops))
        , endpoints(std::move(endpoints)) {
    }
    std::string name;
    std::vector<StopPtr> stops;
    std::vector<StopPtr> endpoints;
};
using BusPtr = const Bus*;

}  // namespace transport_catalogue