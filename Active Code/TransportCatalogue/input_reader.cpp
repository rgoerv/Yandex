#include "input_reader.h"
#include "geo.h"
#include "stat_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>
#include <string_view>
#include <tuple>

//
#include <fstream>

void Read()
{
    using namespace input;

    std::ifstream file("input.txt");
    auto [input, reader] = fill_input(file);
    output::release_output(input, reader);
    //output::release_output(input, reader);
}

namespace input {

using std::istream;
using std::string;
using std::string_view;
using std::vector;

using namespace std::string_literals;
using namespace Catalogue;

std::tuple<istream&, Reader&> fill_input(istream& input)
{
    size_t size = 0;
    input >> size;
    Reader reader(size);

    return { reader.GetInput(input), reader };
}

vector<string_view> SplitIntoWords(string_view str)
{
    vector<string_view> result;

    size_t space = str.find_first_not_of(' ');
    size_t after_space = str.find_first_of(' ', space);

    while (space != str.npos) {
        result.push_back(std::move(str.substr(space, after_space - space)));
        str.remove_prefix(std::min(str.size(), after_space));
        space = str.find_first_not_of(' ');
        after_space = str.find_first_of(' ', space);
    }
    return result;
}

vector<string_view> SplitIntoStops(string_view line, const char delim)
{
    vector<string_view> result;
    size_t stopname_end = line.find(delim);

    while (stopname_end != line.npos) {
        result.push_back(line.substr(0, stopname_end - 1));
        line.remove_prefix(stopname_end + 2);
        stopname_end = line.find(delim);
    }

    result.push_back(line.substr(0, line.find_last_not_of(' ') + 1));
    return result;
}

Reader::Reader(size_t size)
{
    input_queries_.reserve(size);
}

istream& Reader::GetInput(istream& input)
{
    string line;
    for (auto i = 0; i < 10 && getline(input, line); ++i) {
        input_queries_.push_back(std::move(line));
    }
    FillCatalogue();
    return input;
}

void Reader::FillCatalogue()
{
    size_t i = 0;
    for (auto it = input_queries_.begin(); it != input_queries_.end(); ++it, ++i) {
        (*it).substr(0, 4) == "Stop"s ? AddStop(*it) : bus_queries_.push_back(i); // check 3 or 4
    }

    for (const auto index : bus_queries_) {
        AddBus(input_queries_[index]);
    }
}

const TransportCatalogue& Reader::GetCatalogue()
{
    return catalogue;
}

void Reader::AddStop(string_view query_line)
{
    vector<string_view> query(SplitIntoWords(query_line));
    query[1].remove_suffix(1);
    query[2].remove_suffix(1);
    double lat = std::stod(static_cast<string>(query[2]));
    double lng = std::stod(static_cast<string>(query[3]));
        
    catalogue.AddStop(std::move(static_cast<string>(query[1])), lat, lng);
}

void Reader::AddBus(string_view query_line)
{
    size_t npos = query_line.npos;

    query_line.remove_prefix(4);
    size_t pos_2point = query_line.find(':');
    string name = static_cast<string>(query_line.substr(0, pos_2point));

    query_line.remove_prefix(pos_2point + 2);
    const char delim = query_line.find('>') == npos ? '-' : '>';

    vector<string_view> stops(SplitIntoStops(query_line, delim));
    vector<string_view> full_route(stops);

    if (delim == '-') 
    {
        full_route.reserve(2 * stops.size());
        for (size_t i = stops.size() - 2; i != 0; --i) {
            full_route.push_back(stops[i]);
        }
    }

    vector<const Stop*> result;
    result.reserve(full_route.size());

    for (const auto& stop : full_route) {
        result.push_back(catalogue.FindStop(stop));
    }
}

} // namespace input
