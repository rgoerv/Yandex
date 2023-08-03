#pragma once

namespace input {
class Reader;
}

#include "geo.h"
#include "stat_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>
#include <string_view>
#include <tuple>

void Read(std::istream& in, std::ostream& out);

namespace input {

using namespace std::string_literals;
using namespace Catalogue;

std::istream& FillInput(std::istream& input, Reader& reader);

class Reader {
public:
    void Reserve(size_t size);

    std::istream& GetInput(std::istream& input);
    void FillCatalogue();

    TransportCatalogue& GetCatalogue();

private:
    std::vector<std::string> input_queries_;
    std::unordered_map<std::string_view, std::string_view> stop_to_distance_queries_;
    std::vector<size_t> bus_queries_;
    TransportCatalogue catalogue;

    void AddStop(std::string_view query_line);
    void AddBus(std::string_view query_line);
};

} // namespace input