#pragma once

#include "domain.h"

#include <deque>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace transport_catalogue {

namespace detail {

struct PtrHasher {
    template <typename T>
    size_t operator()(const T* ptr) const {
        return hasher(ptr);
    }

    std::hash<const void*> hasher;
};

template <typename HasherL, typename HasherR>
struct PairHasher {
    template <typename T, typename S>
    size_t operator()(const std::pair<T, S>& obj) const {
        return hasher_l(obj.first) + 37 * hasher_r(obj.second);
    }

    HasherL hasher_l;
    HasherR hasher_r;
};

}  // namespace detail

struct BusStat {
    size_t total_stops = 0;
    size_t unique_stops = 0;
    double route_length = 0.;
    double route_length_direct = 0.;
};

class TransportCatalogue {
public:
    using BusPool = std::deque<Bus>;

    const BusPool& GetBuses() const {
        return bus_pool_;
    }

    using StopPool = std::deque<Stop>;

    const StopPool& GetStops() const {
        return stop_pool_;
    }

    StopPtr FindStop(std::string_view stop_name) const;

    BusPtr FindBus(std::string_view bus_name) const;

    void AddStop(std::string name, geo::Coordinates pos);

    void AddBus(std::string name, std::vector<StopPtr> stops, std::vector<StopPtr> endpoints);

    BusStat GetStat(BusPtr bus) const;

    const std::unordered_set<BusPtr>& GetBusesByStop(StopPtr stop) const;

    void SetDistance(StopPtr from, StopPtr to, int meters);

    int GetDistance(StopPtr from, StopPtr to) const;

    using StopPair = std::pair<StopPtr, StopPtr>;
    using StopPairHasher = detail::PairHasher<detail::PtrHasher, detail::PtrHasher>;
    using Distances = std::unordered_map<StopPair, int, StopPairHasher>;

    const Distances& GetDistances() const {
        return distances_;
    }

private:
    BusPool bus_pool_;
    StopPool stop_pool_;

    std::unordered_map<std::string_view, BusPtr> bus_by_name_;
    std::unordered_map<std::string_view, StopPtr> stop_by_name_;
    std::unordered_map<StopPtr, std::unordered_set<BusPtr>> bus_by_stop_;

    Distances distances_;
};

class BuildError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class TransportCatalogueBuilder {
public:
    struct BusDesc {
        std::string name;
        std::vector<std::string> stop_names;
        bool is_roundtrip = false;
    };

    struct StopDesc {
        std::string name;
        double latitude = 0.0;
        double longitude = 0.0;
        std::vector<std::pair<std::string, int>> road_distances;
    };

    TransportCatalogueBuilder& AddBus(BusDesc bus_desc);
    TransportCatalogueBuilder& AddStop(StopDesc stop_desc);

    [[nodiscard]] TransportCatalogue Build() const;

private:
    std::deque<BusDesc> bus_descriptions_;
    std::deque<StopDesc> stop_descriptions_;
};

}  // namespace transport_catalogue