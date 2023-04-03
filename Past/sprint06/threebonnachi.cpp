#include <cstdint>
#include <iostream>

#include "log_duration.h"

using namespace std;

// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
int64_t T(int i) {
    LOG_DURATION("T"s);
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }
    int64_t prev0 = 0;
    int64_t prev1 = 0;
    int64_t prev2 = 1;
    for(int64_t j = 1; j < i; ++j) {
        int64_t next = prev0 + prev1 + prev2;
        prev0 = prev1;
        prev1 = prev2;
        prev2 = next;
    }

    return prev1;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}  