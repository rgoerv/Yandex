#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    // место для вашего решения
    vector<float> results = {};

    const size_t measures_in_day = (measures.empty() ? 0 : measures.at(0).size());
    results.reserve(measures_in_day);

    vector<float> avg = {};
    size_t days = measures.size();
    for(size_t measure = 0; measure < measures_in_day; ++measure){
        float sum_from_avg = 0;
        int64_t positive = 0;
        for(size_t day = 0; day < days; ++day){
            // cout << day << " " << measure << " " <<  measures[day][measure] << endl;
            sum_from_avg += (measures[day][measure] >= 1 ? measures[day][measure] : 0);
            measures[day][measure] >= 1 ? 0 : ++positive;
            // cout << "sum_from_avg inside: " << sum_from_avg << endl;
        }
        // cout << "negatives results: " << positive << endl;
        const auto counts = days - positive;
        counts > 0 ? results.push_back(sum_from_avg / counts) : results.push_back(0);
        // cout << "sum_from_avg: " << sum_from_avg << endl;
        // cout << "result: " << sum_from_avg / counts << endl;
    }
    return results;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
        // data.push_back({vector<float>(5000, 0)});
    }

    // vector<vector<float>> v = {
    //     {0, -1, -1},
    //     {1, -2, -2},
    //     {2, 3, -3},
    //     {3, 4, -4}
    // };

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}