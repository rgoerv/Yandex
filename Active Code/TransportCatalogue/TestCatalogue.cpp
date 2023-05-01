#include "TestCatalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "C:\Programs\Yandex\Past\sprint09\test_framework.h"

// config MVSC : Project properties : Debug -> Command Arguments
// < "C:\Programs\Yandex\Active Code\TransportCatalogue\txts\input.txt" > "C:\Programs\Yandex\Active Code\TransportCatalogue\txts\result_case.txt"  
// 

namespace test {

void Test1()
{
    using namespace input;
    std::ifstream file_input("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsB_case1_input.txt");
    Reader reader;
    istream& input = fill_input(file_input, reader);
    output::release_output(input, reader.GetCatalogue());

    std::ifstream file_output("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsB_case1_output.txt");
    if (!file_output.is_open())
    {
        throw;
    }

    std::ifstream file_result("C://Programs//Yandex//Active Code//TransportCatalogue//txts//result_case.txt");
    if (!file_result.is_open())
    {
        throw;
    }

    string lineout, lineresult;
    for (; getline(file_output, lineout) && getline(file_result, lineresult);) {
        //std::cerr << lineout << std::endl
        //          << lineresult << std::endl;
        assert(lineout == lineresult);
    }
    file_input.close();
    file_result.close();
}

void Test2()
{
    using namespace input;
    std::ifstream file_input("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsB_case2_input.txt");
    Reader reader;
    istream& input = fill_input(file_input, reader);
    output::release_output(input, reader.GetCatalogue());

    std::ifstream file_output("C://Programs//Yandex//Active Code//TransportCatalogue//txts//tsB_case2_output.txt");
    if (!file_output.is_open()) {
        throw;
    }

    std::ifstream file_result("C://Programs//Yandex//Active Code//TransportCatalogue//txts//result_case.txt");
    if (!file_result.is_open()) {
        throw;
    }

    string lineout, lineresult;
    for (; getline(file_output, lineout) && getline(file_result, lineresult);) {
        std::cerr << lineout << std::endl
                  << lineresult << std::endl;
        assert(lineout == lineresult);
    }
    file_input.close();
    file_result.close();
}

} // namespace test