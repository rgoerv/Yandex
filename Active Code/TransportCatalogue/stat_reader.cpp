#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <iostream>
#include <string>

namespace output {

using std::istream;
using std::string;
using std::vector;
using std::cout;
using std::endl;

using namespace input;


void release_output(istream& input, Reader& reader)
{
    size_t size = 0;
    input >> size;

    vector<string> buses(size);

    for (size_t i = 0; i < size; ++i)
    {
        string line;
        getline(input, line);
        buses[i] = line.substr(4, line.find_last_not_of(' '));
    }

    for (const auto& bus : buses)
    {
        cout << reader.GetCatalogue().FindBus(bus)->name_ << endl;
    }
}

} // namespace output