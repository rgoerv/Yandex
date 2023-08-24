#include "transport_catalogue.h"
#include <transport_catalogue.pb.h>
#include <cassert>

using namespace std;

namespace transport_catalogue {

namespace {

std::vector<StopPtr> CreateEndpoints(const std::vector<std::string>& stop_names,
                                     const TransportCatalogue& catalogue) {
    std::vector<StopPtr> endpoints;
    if (stop_names.empty()) {
        return endpoints;
    }
    endpoints.reserve(2);
    if (const StopPtr endpoint1 = catalogue.FindStop(stop_names.front())) {
        endpoints.push_back(endpoint1);
        if (stop_names.front() == stop_names.back()) {
            return endpoints;
        }
        if (const StopPtr endpoint2 = catalogue.FindStop(stop_names.back())) {
            endpoints.push_back(endpoint2);
            return endpoints;
        }
    }
    throw BuildError("Failed to build endpoints"s);
}

std::vector<StopPtr> CreateBusStops(const std::vector<std::string>& stop_names, bool is_roundtrip,
                                    const TransportCatalogue& catalogue) {
    std::vector<StopPtr> bus_stops;

    if (is_roundtrip) {
        bus_stops.reserve(stop_names.size());
    } else if (!stop_names.empty()) {
        bus_stops.reserve(stop_names.size() * 2 - 1);  // end-stop is not repeated
    }

    for (const std::string& stop_name : stop_names) {
        if (const StopPtr& stop = catalogue.FindStop(stop_name)) {
            bus_stops.push_back(stop);
        } else {
            throw BuildError("Stop '"s + stop_name + "' not_found"s);
        }
    }

    if (is_roundtrip || stop_names.size() <= 1) {
        return bus_stops;
    }
    bus_stops.insert(bus_stops.end(), std::next(bus_stops.rbegin()), bus_stops.rend());

    return bus_stops;
}

template <typename Map>
auto GetValueOr(const Map& map, const typename Map::key_type& key,
                const typename Map::mapped_type& default_value) {
    const auto it = map.find(key);
    return (it != map.end()) ? it->second : default_value;
}

}  // namespace

BusStat TransportCatalogue::GetStat(BusPtr bus) const {
    BusStat stat;

    unordered_set<StopPtr> seen_stops;
    StopPtr prev_stop = nullptr;

    for (auto stop : bus->stops) {
        ++stat.total_stops;

        if (seen_stops.count(stop) == 0) {
            ++stat.unique_stops;
            seen_stops.insert(stop);
        }

        if (prev_stop != nullptr) {
            stat.route_length_direct += geo::ComputeDistance(prev_stop->position, stop->position);
            stat.route_length += GetDistance(prev_stop, stop);
        }

        prev_stop = stop;
    }

    return stat;
}

StopPtr TransportCatalogue::FindStop(std::string_view stop_name) const {
    return GetValueOr(stop_by_name_, stop_name, nullptr);
}

BusPtr TransportCatalogue::FindBus(std::string_view bus_name) const {
    return GetValueOr(bus_by_name_, bus_name, nullptr);
}

void TransportCatalogue::AddStop(std::string name, geo::Coordinates pos) {
    stop_pool_.emplace_back(std::move(name), pos);
    auto added_ptr = &stop_pool_.back();
    stop_by_name_[added_ptr->name] = added_ptr;
}

void TransportCatalogue::AddBus(std::string name, std::vector<StopPtr> stops,
                                std::vector<StopPtr> endpoints) {
    bus_pool_.emplace_back(std::move(name), std::move(stops), std::move(endpoints));
    auto added_ptr = &bus_pool_.back();
    bus_by_name_[added_ptr->name] = added_ptr;

    for (auto stop : added_ptr->stops) {
        bus_by_stop_[stop].insert(added_ptr);
    }
}

const std::unordered_set<BusPtr>& TransportCatalogue::GetBusesByStop(StopPtr stop) const {
    static const std::unordered_set<BusPtr> dummy;

    auto iter = bus_by_stop_.find(stop);

    return iter == bus_by_stop_.end() ? dummy : iter->second;
}

void TransportCatalogue::SetDistance(StopPtr from, StopPtr to, int meters) {
    distances_[{from, to}] = meters;
}

int TransportCatalogue::GetDistance(StopPtr from, StopPtr to) const {
    auto iter = distances_.find({from, to});
    if (iter == distances_.end()) {
        iter = distances_.find({to, from});

        if (iter == distances_.end()) {
            return -1;
        }
    }

    return iter->second;
}

TransportCatalogueBuilder& TransportCatalogueBuilder::AddBus(BusDesc bus_desc) {
    bus_descriptions_.emplace_back(std::move(bus_desc));
    return *this;
}

TransportCatalogueBuilder& TransportCatalogueBuilder::AddStop(StopDesc stop_desc) {
    stop_descriptions_.emplace_back(std::move(stop_desc));
    return *this;
}

TransportCatalogue TransportCatalogueBuilder::Build() const {
    TransportCatalogue db;
    for (const StopDesc& stop_desc : stop_descriptions_) {
        db.AddStop(stop_desc.name, {stop_desc.latitude, stop_desc.longitude});
    }

    for (const StopDesc& stop_desc : stop_descriptions_) {
        const StopPtr this_stop = db.FindStop(stop_desc.name);
        assert(this_stop != nullptr);

        for (const auto& [stop_name, distance] : stop_desc.road_distances) {
            if (const StopPtr neighbor = db.FindStop(stop_name)) {
                db.SetDistance(this_stop, neighbor, distance);
            } else {
                throw BuildError("Stop '"s + stop_name + "' not found"s);
            }
        }
    }
    for (const BusDesc& bus_desc : bus_descriptions_) {
        db.AddBus(bus_desc.name, CreateBusStops(bus_desc.stop_names, bus_desc.is_roundtrip, db),
                  CreateEndpoints(bus_desc.stop_names, db));
    }

    return db;
}

}  // namespace transport_catalogue