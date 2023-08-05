#include "budget_manager.h"
#include "parser.h"

#include <iostream>
#include <string_view>
#include <regex>
#include <cstdint>

using namespace std;

TimePeriod ParsePeriod(std::string_view str) {
    TimePeriod period;

    size_t space = str.find_first_of(' ');

    period.begin = Date::FromString(str.substr(0, space));
    str.remove_prefix(space + 1);

    period.end = Date::FromString(str.substr(0, str.npos));

    return period;
}

void ParseComputeIncome(Manager::BudgetManager& budget, std::string_view str) {
    budget.ComputeIncome(ParsePeriod(str));
}

void ParseEarn(Manager::BudgetManager& budget, std::string_view str) {
    size_t income_begin = str.find_last_of(' ');

    budget.Earn(ParsePeriod(str.substr(0, income_begin)),
        std::stod(static_cast<std::string>(str.substr(income_begin, str.npos))));
}

void ParsePayTax(Manager::BudgetManager& budget, std::string_view str) {
    size_t income_begin = str.find_last_of(' ');

    budget.PayTax(ParsePeriod(str.substr(0, income_begin)), 
        std::stod(static_cast<std::string>(str.substr(income_begin, str.npos))));
}

void ParseSpend(Manager::BudgetManager& budget, std::string_view str) {
    size_t income_begin = str.find_last_of(' ');

    budget.Spend(ParsePeriod(str.substr(0, income_begin)),
        std::stod(static_cast<std::string>(str.substr(income_begin, str.npos))));
}

// функция чтения и обработки запроса
void ParseAndProcessQuery(Manager::BudgetManager& manager, string_view line) {

    size_t header_end = line.find_first_of(' ');
    string header = static_cast<string>(line.substr(0, header_end));

    if (header == "Earn"s) {
        ParseEarn(manager, line.substr(header_end + 1, line.npos));
    }
    else if (header == "ComputeIncome"s) {
        ParseComputeIncome(manager, line.substr(header_end + 1, line.npos));
    }
    else if (header == "PayTax"s) {
        ParsePayTax(manager, line.substr(header_end + 1, line.npos));
    }
    else if (header == "Spend"s) {
        ParseSpend(manager, line.substr(header_end + 1, line.npos));
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    Manager::BudgetManager manager;

    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }
}
