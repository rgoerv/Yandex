#pragma once

#include "geo.h"
#include "domain.h"

#include <deque>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>
#include <set>

namespace Catalogue {

using namespace domain;

class TransportCatalogue {
public:
    void AddStop(const std::string& name, double lat, double lng);
    const Stop* FindStop(std::string_view name) const;
    void AddBus(const std::string& name, const std::vector<const Stop*>& route,
        int64_t length, double geo_length, bool is_roundtrip, const Stop* last_stop);
    const Bus* FindBus(std::string_view name) const;
    const BusInfo GetBusInfo(std::string_view name) const;
    bool CheckStop(std::string_view name) const;
    const std::set<std::string_view>& GetBusesInStop(std::string_view stopname) const;
    void AddDistance(std::string_view stop_x, std::string_view stop_y, int64_t distance);
    int64_t GetDistance(const Stop* from, const Stop* to) const;
    int64_t GetDistance(const Bus* bus, size_t start, size_t count) const;

    const std::unordered_map<std::string_view, const Bus*>& GetBusNameToBus() const;
    const std::unordered_map<const Stop*, std::set<std::string_view>>& GetStopToBuses() const;
    const std::deque<Bus>& GetBuses() const;
    size_t GetStopCount() const;


private:
    std::deque<Stop> stops_;
    // удобный доступ stopname из stops_ и указатель на Stop
    std::unordered_map<std::string_view, const Stop*> stopname_to_stop_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
    // 
    std::unordered_map<const Stop*, std::set<std::string_view>> stop_to_buses_; 
    // расстояние между двумя остановками
    std::unordered_map<std::pair<const Stop*, const Stop*>, int64_t, HacherPair> dist_btn_stops_;
};

} // namespace Catalogue