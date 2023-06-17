#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <iostream>
#include <iomanip>
#include <string>

#include <fstream>

namespace output {

using namespace std::string_literals;

void PrintBus(std::ostream& output, std::string_view bus, const TransportCatalogue& catalogue)
{
    output << "Bus "s << bus << ": "s;
    auto [check, size, unique_size, distance, geo_distance] = catalogue.GetBusInfo(bus);
    if (!check) {
        output << "not found"s << std::endl;
        return;
    }
    output << size << " stops on route, "s << unique_size << " unique stops, "s
           << std::setprecision(6) << static_cast<double>(distance) << " route length, "s 
           << static_cast<double>(distance / geo_distance) << " curvature"s << std::endl;
}

void PrintBusesInStop(std::ostream& output, std::string_view stop, const TransportCatalogue& catalogue)
{
    output << "Stop "s << stop << ": "s;
    if (!catalogue.CheckStop(stop)) {
        output << "not found"s << std::endl;
        return;
    }

    const std::set<std::string_view>& buses = catalogue.GetBusesInStop(stop);
    if (buses.empty()) {
        output << "no buses"s << std::endl;
        return;
    }

    output << "buses"s;

    for (const auto& bus : buses) {
        output << " "s << bus;
    }
    output << std::endl;
}

void ExecuteQueries(std::istream& input, std::ostream& output, const TransportCatalogue& catalogue)
{
    size_t size = 0;
    input >> size;
    input.ignore();

    for (size_t i = 0; i < size; ++i)
    {
        std::string line;
        getline(input, line);
        
        if (line.substr(0, 4) != "Stop"s)
        {
            PrintBus(output, line.substr(4, line.npos), catalogue);
        }
        else
        {
            PrintBusesInStop(output, line.substr(5, line.npos), catalogue);
        }
    }
}

} // namespace output