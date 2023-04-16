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

//
#include <fstream>

void Read();

namespace input {

class Reader;

using std::istream;
using std::string;
using std::string_view;
using std::vector;

using namespace std::string_literals;
using namespace Catalogue;

std::tuple<istream&, Reader&> fill_input(istream& input);
vector<string_view> SplitIntoWords(string_view str);
vector<string_view> SplitIntoStops(string_view line, const char delim);

class Reader {
public:
    Reader(size_t size);

    istream& GetInput(istream& input);

    void FillCatalogue();
private:
    vector<string> input_queries_;
    vector<size_t> bus_queries_;
    TransportCatalogue catalogue;

    void AddStop(string_view query_line);
    void AddBus(string_view query_line);
};

} // namespace input