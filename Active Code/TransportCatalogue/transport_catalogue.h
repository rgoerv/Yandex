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

using std::string;
using std::string_view;
using std::vector;
using std::deque;
using std::unordered_map;
using std::set;
using std::pair;

struct Stop {
    string name_; // название остановки
    Coordinates coordinates; // координаты остановки
    explicit Stop(string name, double latit, double longt)
        : name_(name)
        , coordinates({latit, longt})
    {
    }
};

struct Bus {
    string name_; // название маршрута
    vector<const Stop*> route_; // маршрут по остановкам
    int64_t unique_size = 0;
    double length_ = 0;
    explicit Bus(string name, vector<const Stop*> route, int64_t size, double length)
        : name_(name)
        , route_(route)
        , unique_size(size)
        , length_(length)
    {
    }
    bool CheckStop(const Stop* stop) const;
};

struct HacherPair
{
    size_t operator()(const pair<const Stop*, const Stop*>& stops) const
    {
        return std::hash<const void*> {}(stops.first)
            + std::hash<const void*> {}(stops.second) * 37;
    }
};

class TransportCatalogue {
public:
    void AddStop(string name, double lat, double lng);
    const Stop* FindStop(string_view name) const;
    void AddBus(string name, vector<const Stop*> route, double length);
    const Bus* FindBus(string_view name) const;
    std::tuple<bool, int64_t, int64_t, double> GetBusInfo(string_view name) const;
    bool CheckStop(string_view name) const;
    const set<string_view>& GetBusesInStop(string_view stopname) const;
    void AddDistance(string_view stop_x, string_view stop_y, double distance);

private:
    deque<Stop> stops_;
    // удобный доступ stopname из stops_ и указатель на Stop
    unordered_map<string_view, const Stop*> stopname_to_stop_;
    deque<Bus> buses_;
    unordered_map<string_view, const Bus*> busname_to_bus_;
    // 
    unordered_map<const Stop*, set<string_view>> stop_to_buses_; 
    // расстояние между двумя остановками
    unordered_map<pair<const Stop*, const Stop*>, double, HacherPair> dist_btn_stops_;
};

} // namespace Catalogue