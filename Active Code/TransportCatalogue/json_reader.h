#pragma once

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

#include "transport_catalogue.h"
#include "geo.h"
#include "domain.h"
#include "json.h"

#include <utility>
#include <variant>

namespace JsonReader {

using namespace std::literals;
using namespace Catalogue;
using namespace json;
using namespace domain;

class Reader {
public:
    Reader(std::istream& input);
    void Reply(std::ostream& output) const;
    const TransportCatalogue& GetCatalogue() const;
    const json::Document GetRenderSettings() const;

private:
    json::Document document;
    TransportCatalogue catalogue;
    std::unordered_map<std::pair<std::string_view, std::string_view>, int64_t, HacherPair> stops_to_dstns;
    Array stat_response;

    void BaseRequestHandle();
    void StopBaseRequestHandle(const Array& base_requests);
    void BusBaseRequestHandle(const Array& base_reauest);
    void AgreeDistances(const std::vector<const domain::Stop*>& stops, int64_t& lenght, double& geo_length) const;

    void StatRequestHandle();
    void StopStatRequestHandle(const Node& request, int id);
    void BusStatRequestHandle(const Node& request, int id);
};

} // namespace JsonReader