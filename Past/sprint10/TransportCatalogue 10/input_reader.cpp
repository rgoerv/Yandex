#include "input_reader.h"
#include "geo.h"
#include "stat_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>
#include <string_view>
#include <tuple>

void Read(std::istream& in, std::ostream& out)
{
    using namespace input;
    Reader reader;
    std::istream& input = FillInput(in, reader);
    output::ExecuteQueries(in, out, reader.GetCatalogue());
}

namespace input {

using namespace std::string_literals;
using namespace Catalogue;

namespace Spliting 
{

std::vector<std::pair<int64_t, std::string_view>> SplitIntoDistStop(std::string_view line)
{
    std::vector<std::pair<int64_t, std::string_view>> result;
    size_t comma = line.find(',');

    // 5600m to Rossoshanskaya ulitsa
    while (comma != line.npos)
    {
        size_t m = line.find('m');
        size_t stop_begin = m + 5;
        result.push_back({ std::stoi(static_cast<std::string>(line.substr(0, m))), 
                            line.substr(stop_begin, comma - stop_begin) });
        line.remove_prefix(comma + 2);
        comma = line.find(',');
    }
    size_t m = line.find('m');
    if (m != line.npos)
    {
        size_t stop_begin = m + 5;
        result.push_back({ std::stoi(static_cast<std::string>(line.substr(0, m))),
            line.substr(stop_begin, comma) });
    }

    return result;
}

std::vector<std::string_view> SplitStop(std::string_view str)
{
    std::vector<std::string_view> result;

    str.remove_prefix(5);
    size_t name_end = str.find(':');
    result.push_back(str.substr(0, name_end)); // stopname 0

    str.remove_prefix(name_end + 2);
    size_t comma = str.find(',');
    result.push_back(str.substr(0, comma)); // latitude 1

    str.remove_prefix(comma + 2);
    comma = str.find(',');
    result.push_back(str.substr(0, comma)); // longitude 2

    str.remove_prefix(comma + 2);
    result.push_back(str.substr(0, str.npos)); // distance"m to "stopname

    return result;
}

std::vector<std::string_view> SplitBus(std::string_view line, const char delim)
{
    std::vector<std::string_view> result;
    size_t stopname_end = line.find(delim);

    while (stopname_end != line.npos) {
        result.push_back(line.substr(0, stopname_end - 1));
        line.remove_prefix(stopname_end + 2);
        stopname_end = line.find(delim);
    }

    result.push_back(line.substr(0, line.find_last_not_of(' ') + 1));
    return result;
}

} // namespace Spliting

std::istream& FillInput(std::istream& input, Reader& reader)
{
    size_t size = 0;
    input >> size;
    input.ignore();
    
    reader.Reserve(size);
    std::istream& out_input = reader.GetInput(input);

    return out_input;
}

void Reader::Reserve(size_t size)
{
    input_queries_.reserve(size);
}

std::istream& Reader::GetInput(std::istream& input)
{
    std::string line;
    
    for (auto i = 0; i < input_queries_.capacity() && getline(input, line); ++i) {
        input_queries_.push_back(line);
    }
    FillCatalogue();
    return input;
}

void Reader::FillCatalogue()
{
    size_t i = 0;
    for (auto it = input_queries_.begin(); it != input_queries_.end(); ++it, ++i) {
        (*it).substr(0, 4) == "Stop"s ? AddStop(*it) : bus_queries_.push_back(i);
    }

    for (const auto& [stop, qline] : stop_to_distance_queries_) {
        auto distances_to_stops = Spliting::SplitIntoDistStop(qline);
        for (const auto& [distance, stopline] : distances_to_stops)
        {
            catalogue.AddDistance(stop, stopline, distance);
        }
    }
    for (const auto index : bus_queries_) {
        AddBus(input_queries_[index]);
    }
}

TransportCatalogue& Reader::GetCatalogue()
{
    return catalogue;
}

void Reader::AddStop(std::string_view query_line)
{
    std::vector<std::string_view> query(Spliting::SplitStop(query_line));

    double lat = std::stod(static_cast<std::string>(query[1]));
    double lng = std::stod(static_cast<std::string>(query[2]));
    std::string stop_name = static_cast<std::string>(query[0]);
    stop_to_distance_queries_.insert({ query[0], query[3] });
     
    catalogue.AddStop(std::move(stop_name), lat, lng);
}

void Reader::AddBus(std::string_view query_line)
{
    size_t npos = query_line.npos;

    query_line.remove_prefix(4);
    size_t pos_2point = query_line.find(':');
    std::string name = static_cast<std::string>(query_line.substr(0, pos_2point));

    query_line.remove_prefix(pos_2point + 2);
    const char delim = query_line.find('>') == npos ? '-' : '>';

    std::vector<std::string_view> stops(Spliting::SplitBus(query_line, delim));
    std::vector<std::string_view> full_route(stops);

    if (delim == '-')
    {
        full_route.reserve(2 * stops.size());
        for (int64_t i = stops.size() - 2; i > -1; --i) {
            full_route.push_back(stops[i]);
        }
    }

    std::vector<const Stop*> result;
    result.reserve(full_route.size());

    double geo_length = .0;
    int64_t length = 0;
    const Stop* first = catalogue.FindStop(full_route[0]);
    const Stop* last;

    for (const auto& stop : full_route) {
        last = catalogue.FindStop(stop);
        
        geo_length += ComputeDistance(first->coordinates, last->coordinates);
        result.push_back(catalogue.FindStop(stop));

        const int64_t distance =  catalogue.GetDistance(first, last);
        length += distance > 0 ? distance : catalogue.GetDistance(last, first);
        
        first = last;
    }

    catalogue.AddBus(name, result, length, geo_length);
}

} // namespace input
