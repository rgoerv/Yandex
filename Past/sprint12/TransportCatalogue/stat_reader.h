#pragma once

#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>

namespace output {

using namespace input;
using namespace Catalogue;

void PrintBus(std::ostream& output, std::string_view bus, const TransportCatalogue& catalogue);
void PrintBusesInStop(std::ostream& output, std::string_view stop, const TransportCatalogue& catalogue);
void ExecuteQueries(std::istream& input, std::ostream& output, const TransportCatalogue& catalogue);

} // namespace output