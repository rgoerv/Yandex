#include "input_reader.h"
#include "stat_reader.h"
#include "geo.h"

#include <deque>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>

using std::string;
using std::string_view;

class TransportCatalogue 
{
    struct Stop
    {
        string name_;
        Coordinates coordinates_;
    };
    struct Bus
    {
        string name_; // название маршрута
        std::vector<Stop*> route_; // маршрут по остановкам
    };
public:
    void AddStop(string name, Coordinates locate) {
        Stop stop(name, locate);
        stops_.push_back(stop);
        stopname_to_stop_.insert(stop.name_, &stop);
    }

    void FindStop()
    {

    }

    void AddBus()
    {

    }

    void FindBus()
    {

    }

    void GetBusInfo()
    {

    }

private:
    std::deque<Stop> stops_;
    std::unordered_map<string_view, Stop*> stopname_to_stop_; // удобный доступ stopname из stops_ и указатель на Stop
    std::deque<Bus> buses_;
    std::unordered_map<string_view, Bus*> busname_to_bus_;
    std::unordered_map<std::pair<Stop*, Stop*>, double> dist_btn_stops_; // расстояние между двумя остановками
};