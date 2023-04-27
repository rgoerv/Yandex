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
    double lat;
    double lng; // координаты остановки
    explicit Stop(string name, double latit, double longt)
        : name_(name)
        , lat(latit)
        , lng(longt)
    {
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

class TransportCatalogue {
public:
    void AddStop(string name, double lat, double lng);
    const Stop* FindStop(string_view name) const;
    void AddBus(string name, vector<Stop*> route);
    const Bus* FindBus(string_view name) const;
    const Bus* GetBusInfo(string_view name);

private:
    std::deque<Stop> stops_;
    // удобный доступ stopname из stops_ и указатель на Stop
    std::unordered_map<string_view, const Stop*> stopname_to_stop_;
    std::deque<Bus> buses_;
    std::unordered_map<string_view, const Bus*> busname_to_bus_;
    // расстояние между двумя остановками
    // std::unordered_map<std::pair<const Stop*, const Stop*>, double> dist_btn_stops_;
};

} // namespace Catalogue