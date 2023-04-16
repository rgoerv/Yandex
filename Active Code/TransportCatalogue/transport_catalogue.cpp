#include "transport_catalogue.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Catalogue {

void TransportCatalogue::AddStop(string name, Coordinates locate)
{
    stops_.push_back(Stop { name, locate });
    stopname_to_stop_.insert({ stops_.back().name_, &stops_.back() });
}

const Stop* TransportCatalogue::FindStop(string_view name) const
{
    // std::pair<bool, const Stop*>
    // return stopname_to_stop_.count(name) > 0 ? std::make_pair(true, stopname_to_stop_.at(name))
    //                                          : std::make_pair(false, nullptr);
    return stopname_to_stop_.at(name);
}

void TransportCatalogue::AddBus(string name, vector<Stop*> route)
{
    buses_.push_back(Bus { name, route });
    busname_to_bus_.insert({ buses_.back().name_, &buses_.back() });
}

const Bus* TransportCatalogue::FindBus(string_view name) const
{
    // std::pair<bool, const Bus*>
    // return busname_to_bus_.count(name) > 0 ? std::make_pair(true, busname_to_bus_.at(name))
    //                                          : std::make_pair(false, nullptr);
    return busname_to_bus_.at(name);
}

const Bus* TransportCatalogue::GetBusInfo(string_view name)
{

}

} // namespace TransportCatalogue