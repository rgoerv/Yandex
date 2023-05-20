#include "TestCatalogueTextInput.h"
#include "input_reader.h"
#include "stat_reader.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "C:\Programs\Yandex\Past\sprint09\test_framework.h"

// config MVSC : Project properties : Debug -> Command Arguments
// < "C:\Programs\Yandex\Active Code\TransportCatalogue\txts\input.txt" > "C:\Programs\Yandex\Active Code\TransportCatalogue\txts\result_case.txt"  
// no actual

namespace test {

void Test()
{
    using namespace input;
    std::ifstream file_input("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsC_case1_input.txt");
    std::ifstream file_output("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsC_case1_output1.txt");
    
    std::ofstream out_result("C://Programs//Yandex//Active Code//TransportCatalogue//txts//result_case.txt");
    
    if (!file_input.is_open()) {
        throw;
    }
    if (!file_output.is_open()) {
        throw;
    }
    if (!out_result.is_open()) {
        throw;
    }

    Read(file_input, out_result);
    out_result.close();

    std::ifstream in_result("C://Programs//Yandex//Active Code//TransportCatalogue//txts//result_case.txt");

    if (!in_result.is_open()){
        throw;
    }

    std::string lineout, lineresult;
    for (; getline(file_output, lineout) && getline(in_result, lineresult);) {
        std::cerr << lineout << std::endl
                  << lineresult << std::endl;
        assert(lineout == lineresult);
    }
    file_input.close();
    file_output.close();
    in_result.close();
}

} // namespace test