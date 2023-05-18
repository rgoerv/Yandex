#include "transport_catalogue.h"
#include "domain.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>

#include <iostream>

namespace Catalogue {

void TransportCatalogue::AddStop(const std::string& name, double lat, double lng)
{
    stops_.push_back(Stop { name, lat, lng });
    stopname_to_stop_.insert({ stops_.back().name_, &stops_.back() });
}

const Stop* TransportCatalogue::FindStop(std::string_view name) const
{
    return stopname_to_stop_.at(name);
}

bool TransportCatalogue::CheckStop(std::string_view name) const
{
    return stopname_to_stop_.count(name) > 0;
}

void TransportCatalogue::AddBus(const std::string& name, const std::vector<const Stop*>& route, int64_t length, double geo_length)
{
    std::set<const Stop*> unique(route.begin(), route.end());
    size_t unique_size = unique.size();

    buses_.push_back(Bus { name, route, unique_size, length, geo_length });
    busname_to_bus_.insert({ buses_.back().name_, &buses_.back() });
    
    std::string_view busname = buses_.back().name_;
    for (const Stop* stop : route)
    {
        stop_to_buses_[stop].insert(busname);
    }
}

const Bus* TransportCatalogue::FindBus(std::string_view name) const
{
    return busname_to_bus_.at(name);
}

const std::unordered_map<std::string_view, const Bus*>& TransportCatalogue::GetBusNameToBus() const {
    return busname_to_bus_;
}

const std::unordered_map<const Stop*, std::set<std::string_view>>& TransportCatalogue::GetStopToBuses() const {
    return stop_to_buses_;
}


const BusInfo TransportCatalogue::GetBusInfo(std::string_view name) const
{
    if (!busname_to_bus_.count(name)) {
        return BusInfo {};
    }

    const Bus* bus = busname_to_bus_.at(name);
    return BusInfo { true, bus->route_.size(), bus->unique_size, bus->length_, bus->geo_length_ };
}

const std::set<std::string_view>& TransportCatalogue::GetBusesInStop(std::string_view stopname) const
{
    const Stop* stop_ptr = FindStop(stopname);
    if (stop_to_buses_.count(stop_ptr) > 0)
    {
        return stop_to_buses_.at(stop_ptr);
    }
    static const std::set<std::string_view> empty;
    return empty;
}

void TransportCatalogue::AddDistance(std::string_view stop_x, std::string_view stop_y, int64_t distance)
{
    dist_btn_stops_[{ FindStop(stop_x), FindStop(stop_y) }] = distance;
}

int64_t TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const
{
    // i want .contains(key) from c++20
    return dist_btn_stops_.count({ from, to }) > 0 ? dist_btn_stops_.at({ from, to }) : 0;
}

} // namespace TransportCatalogue