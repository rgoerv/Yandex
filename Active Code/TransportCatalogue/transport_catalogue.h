#pragma once

#include "geo.h"

#include <deque>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>

namespace Catalogue {

using std::string;
using std::string_view;
using std::vector;

struct Stop {
    string name_; // название остановки
    Coordinates coordinates_; // координаты остановки
    explicit Stop(string name, Coordinates coordinates)
        : name_(name)
        , coordinates_(coordinates)
    {
    }
};

struct StopHasher {
    static const size_t N = 37;
    size_t operator()(const Stop& stop)
    {
        return std::hash<string> {}(stop.name_) + N * 1 * std::hash<double> {}(stop.coordinates_.lat)
            + N * 2 * std::hash<double> {}(stop.coordinates_.lng);
    }
};

struct Bus {
    string name_; // название маршрута
    vector<Stop*> route_; // маршрут по остановкам
    explicit Bus(string name, vector<Stop*> route)
        : name_(name)
        , route_(route)
    {
    }
};

//struct BusHasher {
//    static const size_t N = 37;
//    size_t operator()(const Bus& bus)
//    {
//        return std::hash<string> {}(bus.name_) + N * 1 * std::hash<> {}(stop.coordinates_.lat)
//            + N * 2 * std::hash<double> {}(stop.coordinates_.lng);
//    }
//};



class TransportCatalogue {
public:
    void AddStop(string name, Coordinates locate);
    const Stop* FindStop(string_view name) const;
    void AddBus(string name, vector<Stop*> route);
    const Bus* FindBus(string_view name) const;
    const Bus* GetBusInfo(string_view name);

private:
    std::deque<Stop> stops_;
    // удобный доступ stopname из stops_ и указатель на Stop
    std::unordered_map<string_view, const Stop*, StopHasher> stopname_to_stop_;
    std::deque<Bus> buses_;
    std::unordered_map<string_view, const Bus*> busname_to_bus_;
    // расстояние между двумя остановками
    std::unordered_map<std::pair<const Stop*, const Stop*>, double> dist_btn_stops_;
};

} // namespace Catalogue