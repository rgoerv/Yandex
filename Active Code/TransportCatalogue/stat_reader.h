#pragma once

#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>

namespace output {

using std::istream;
using std::string_view;
using namespace input;
using namespace Catalogue;
void PrintBus(string_view bus, const TransportCatalogue& catalogue);
void PrintBusesInStop(string_view stop, const TransportCatalogue& catalogue);
void release_output(istream& input, const TransportCatalogue& catalogue);
} // namespace output