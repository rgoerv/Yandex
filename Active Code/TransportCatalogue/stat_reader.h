#pragma once

#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"

#include <iostream>
#include <string>

namespace output {

using std::string;
using std::istream;
using std::vector;
using namespace input;
void release_output(istream& input, Reader& reader);
} // namespace output