#include "json_reader.h"

#include <fstream>
#include <iostream>
#include <string_view>

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);
    
    JsonReader::Reader reader(std::cin);
    if (mode == "make_base"sv) {
        reader.MakeBase();
    } else if (mode == "process_requests"sv) {
        reader.ProcessRequests();
    } else {
        PrintUsage();
        return 1;
    }
}