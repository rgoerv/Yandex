#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>
#include <future>
#include <functional>

using namespace std;

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const std::execution::sequenced_policy&,
    RandomAccessIterator range_begin, RandomAccessIterator range_end,
    const Value& value) {
    using namespace std;
    
    auto left_bound = range_begin;
    auto right_bound = range_end;
    
    while (left_bound + 1 < right_bound) {
        const auto middle = left_bound + (right_bound - left_bound) / 2;
        if (*middle < value) {
            left_bound = middle;
        }
        else {
            right_bound = middle;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    }
    else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(RandomAccessIterator range_begin, RandomAccessIterator range_end,
    const Value& value) {
    using namespace std;
    return LowerBound(execution::seq, range_begin, range_end, value);
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const std::execution::parallel_policy&, RandomAccessIterator range_begin,
    RandomAccessIterator range_end, const Value& value) {

    // В 3х диапазонах уже разделили
    // [range_begin; middle_left) U [middle_left; middle_right) U [middle_right; range_end).
    // В цикле cужаем до 1 нужного нам итератора left_bound и right_bound

    if (range_begin == range_end) {
        return range_end;
    }

    auto left_bound = range_begin;
    auto right_bound = range_end;

    // cout << "lb: " << left_bound - range_begin << " rb: " << right_bound - range_begin << endl;

    while (left_bound + 1 < right_bound) {

        auto middle_left = left_bound + std::distance(left_bound, right_bound) / 3;
        auto middle_right = left_bound + std::distance(left_bound, right_bound) * 2 / 3;

        future<bool> _middle = async([middle_left, value]
            { return *middle_left < value; });
        
        // bool last = (*middle_right < value);
        
        if (*middle_right < value) {
            left_bound = middle_right;
        }
        else {
            right_bound = middle_right;
            // bool middle = _middle.get();
/*
            if (_middle.get()) {
                left_bound = middle_left;
            }
            else {
                right_bound = middle_left;
            }*/
            _middle.get() ? left_bound = middle_left : right_bound = middle_left; 
        }

        // cout << "lb: " << left_bound - range_begin << " rb: " << right_bound - range_begin << endl;
    }

    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    }
    else {
        return right_bound;
    }
}

int main() {
    using namespace std;
    const vector<string> strings = { "cat", "dog", "dog", "horse" };

    const vector<string> requests = { "bear", "cat", "deer", "dog", "dogs", "horses" };

    // последовательные версии
    cout << "Request [" << requests[0] << "] - position "
        << LowerBound(strings.begin(), strings.end(), requests[0]) - strings.begin() << endl; // 0
    cout << "Request [" << requests[1] << "] - position "
        << LowerBound(std::execution::seq, strings.begin(), strings.end(), requests[1])
        - strings.begin()
        << endl; // 0
    cout << "Request [" << requests[2] << "] - position "
        << LowerBound(execution::seq, strings.begin(), strings.end(), requests[2])
        - strings.begin()
        << endl; // 1

    // параллельные
    cout << "Request [" << requests[3] << "] - position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[3])
        - strings.begin()
        << endl; // 1
    cout << "Request [" << requests[4] << "] - position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[4])
        - strings.begin()
        << endl; // 3
    cout << "Request [" << requests[5] << "] - position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[5])
        - strings.begin()
        << endl; // 4
    const vector<string> empty_strings = { "hyu" };
    cout << "Request [" << requests[5] << "] - position "
        << LowerBound(execution::par, empty_strings.begin(), empty_strings.end(), requests[5])
        - empty_strings.begin()
        << endl; // empty
    system("pause");
}