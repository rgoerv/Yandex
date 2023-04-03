#include "log_duration.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }
    return res;
}

vector<int> ReverseVector2(const vector<int>& source_vector) {
    vector<int> res;
    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator) {
        res.push_back(*iterator);
    }
    return res;
}

vector<int> ReverseVector3(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

vector<int> ReverseVector4(const vector<int>& source_vector) {
    vector<int> res(source_vector.size());
    // реализация вашего собственного реверсирования
    size_t it_res = 0;
    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator, ++it_res) {
        res[it_res] = *iterator;
    }
    return res;
}

void Operate() {
    vector<int> rand_vector;
    int n;

    cin >> n;
    rand_vector.reserve(n);

    for (int i = 0; i < n; ++i) {
        rand_vector.push_back(rand());
    }

    // код измерения тут
    if(n < 100000){
        {
            vector<int> Naive(rand_vector);
            LOG_DURATION("Naive"s);
            ReverseVector(Naive);
        }
        {
            vector<int> Good(rand_vector);
            LOG_DURATION("Good"s);
            ReverseVector2(Good);
        }
    }
    else {
        {
            vector<int> Good(rand_vector);
            LOG_DURATION("Good"s);
            ReverseVector2(Good);
        }
        {
            vector<int> Best(rand_vector);
            LOG_DURATION("Best"s);
            ReverseVector3(Best);
        }
        {
            vector<int> Your(rand_vector);
            LOG_DURATION("Your"s);
            ReverseVector4(Your);
        }
    }
}

int main() {
    Operate();
    return 0;
}