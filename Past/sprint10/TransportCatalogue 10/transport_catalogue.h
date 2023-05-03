#pragma once

#include "geo.h"

#include <deque>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>
#include <tuple>
#include <set>

namespace Catalogue {

struct Stop {
    std::string name_; // название остановки
    Coordinates coordinates; // координаты остановки
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
    size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const
    {
        return std::hash<const void*> {}(stops.first)
            + std::hash<const void*> {}(stops.second) * 37;
    }
};

class TransportCatalogue {
public:
    void AddStop(const std::string& name, double lat, double lng);
    const Stop* FindStop(std::string_view name) const;
    void AddBus(const std::string& name, const std::vector<const Stop*>& route, int64_t length, double geo_length);
    const Bus* FindBus(std::string_view name) const;
    const BusInfo GetBusInfo(std::string_view name) const;
    bool CheckStop(std::string_view name) const;
    const std::set<std::string_view>& GetBusesInStop(std::string_view stopname) const;
    void AddDistance(std::string_view stop_x, std::string_view stop_y, int64_t distance);
    const int64_t GetDistance(const Stop* from, const Stop* to) const;

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