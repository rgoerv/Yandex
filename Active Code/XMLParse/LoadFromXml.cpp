#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <sstream>

// не меняйте файлы xml.h и xml.cpp
#include "xml.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(spendings.begin(), spendings.end(), 0,
                      [](int current, const Spending& spending) {
                          return current + spending.amount;
                      });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromXml(istream& input) {
    vector<Spending> result;

    Document document = Load(input);
    auto vector_nodes = document.GetRoot().Children();

    for(const auto& node : vector_nodes) {
        result.push_back({node.AttributeValue<string>("category"), 
                            node.AttributeValue<int>("amount")});
    }
    return result;
}

int main() {
    ifstream xmlin("C:\\Programs\\Yandex\\Active Code\\XMLParse\\spendings.xml"s);
    if(xmlin.is_open()) cout << "xml open" << endl;

    const vector<Spending> spendings = LoadFromXml(xmlin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}