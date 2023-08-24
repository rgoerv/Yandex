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

const Stop* TransportCatalogue::AddStop(uint32_t id, const std::string& name, double lat, double lng)
{
    if(stopname_to_stop_.count(name) > 0) {
        return stopname_to_stop_.at(name);
    }
    stops_.push_back(Stop(id, name, lat, lng));
    stopname_to_stop_.insert({ stops_.back().name_, &stops_.back() });
    return &stops_.back();
}

const Stop* TransportCatalogue::FindStop(std::string_view name) const
{
    return stopname_to_stop_.at(name);
}

const Stop* TransportCatalogue::FindStop(uint32_t id) const {
    return &stops_.at(id);
}

bool TransportCatalogue::CheckStop(std::string_view name) const
{
    return stopname_to_stop_.count(name) > 0;
}

void TransportCatalogue::AddBus(const std::string& name, const std::vector<const Stop*>& route,
    int64_t length, double geo_length, bool is_roundtrip, const Stop* last_stop)
{
    std::set<const Stop*> unique(route.begin(), route.end());
    size_t unique_size = unique.size();

    buses_.push_back(Bus { name, route, unique_size, length, geo_length, is_roundtrip, last_stop});
    busname_to_bus_.insert({ buses_.back().name_, &buses_.back() });
    
    std::string_view busname = buses_.back().name_;
    for (const Stop* stop : route)
    {
        stop_to_buses_[stop].insert(busname);
    }
}

void TransportCatalogue::AddBus(Bus&& bus)
{
    buses_.push_back(std::move(bus));
    busname_to_bus_.insert({ buses_.back().name_, &buses_.back() });
    
    std::string_view busname = buses_.back().name_;
    for (const Stop* stop : buses_.back().route_)
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

const std::deque<Bus>& TransportCatalogue::GetBuses() const {
    return buses_;
}

size_t TransportCatalogue::GetBusCount() const {
    return buses_.size();
}

const std::deque<Stop>& TransportCatalogue::GetStops() const {
    return stops_;
}

size_t TransportCatalogue::GetStopCount() const {
    return stops_.size();
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

const std::unordered_map<std::pair<const Stop*, const Stop*>, int64_t, HacherPair>& TransportCatalogue::GetDistances() const {
    return dist_btn_stops_;
}

int64_t TransportCatalogue::GetDistance(const Bus* bus, size_t start, size_t count) const {

    int64_t result = 0;
    const auto& route = bus->route_;

    auto end = start + count < bus->route_.size() ? route.begin() + start + count + 1 : route.end();
    auto prev = route.begin() + start;

    for (auto stop = route.begin() + start + 1; stop != end; ++stop) {

        int64_t dst = GetDistance(*prev, *stop);
        if (dst == 0 && bus->is_roundtrip_) {
            dst = GetDistance(*stop, *prev);
        }
        result += dst;
        prev = stop;
    }
    return result;
}

} // namespace TransportCatalogue