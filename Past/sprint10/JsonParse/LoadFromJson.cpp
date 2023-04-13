#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>

// не меняйте файлы json.h и json.cpp
#include "json.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(
        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
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

vector<Spending> LoadFromJson(istream& input) {
    vector<Spending> result;

    Document document = Load(input);
    auto vector_nodes = document.GetRoot().AsArray();

    int counter = 0;
    Spending spend;

    for(const auto& node : vector_nodes) {
        for(const auto& [str, node] : node.AsMap()){
            str == "amount"s ? (void)(spend.amount = node.AsInt()) : (void)(spend.category = node.AsString());
            ++counter;
            if(counter == 2){
                counter = 0;
                result.push_back(spend);
            }
        }
    }
    return result;
}

int main() {
    // не меняйте main
    ifstream jsin("C:\\Programs\\Yandex\\Active Code\\JsonParse\\spendings.json"s);
    if(jsin.is_open()) cout << "json open" << endl;

    const vector<Spending> spendings = LoadFromJson(jsin);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}