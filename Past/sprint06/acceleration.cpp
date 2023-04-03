#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }
    return res;
}

void AppendRandom(vector<int>& v, int n) {
    v.reserve(n);
    for (int i = 0; i < n; i += 15) {
        int number = rand();
        int count = min(15, n - i);
        for (int j = 0; j < count; ++j)
            v.push_back((number >> j) % 2);
    }
}

void Operate() {
    LOG_DURATION("Total"s);

    vector<int> random_bits;

    // операции << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    
    {
        LOG_DURATION("Append random"s);
        AppendRandom(random_bits, N);
    }

    // перевернём вектор задом наперёд
    vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);
        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        // посчитаем процент единиц на начальных отрезках вектора
        int prev_sum = 0;
        int prev_i = 0;
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            const int sum = prev_sum + CountPops(reversed_bits, prev_i, i);
            cout << "After "s << i << " digits we found "s << (sum * 100. / i) << "% pops"s << endl;
            prev_i = i;
            prev_sum = sum;
        }
    }
}

int main() {
    Operate();
}