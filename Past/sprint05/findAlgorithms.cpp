#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <vector>
#include <string>

template <class RandomIt>
std::pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, std::string prefix) {
    using namespace std;

    RandomIt begin = range_begin;
    range_begin = lower_bound(range_begin, range_end, prefix);

    string new_prefix;
    if(prefix.length() > 1){
//        cout << ">1" << endl;
        char last = prefix[prefix.length() - 1];
        size_t size = prefix.length() - 1;
        prefix.resize(size);
        new_prefix = prefix + string(1, static_cast<char>(last + 1));
//        cout << new_prefix << endl;
    }
    else {
        new_prefix = prefix;
    }

    range_end = lower_bound(begin, range_end, new_prefix);

    return make_pair(range_begin, range_end);
}

int main() {
    using namespace std;
    const vector<string> sorted_strings = {"arhangelsk"s, "maicop"s,  "moscow"s, "mormansk"s, "orenburg"s, "vologda"s, "western"s, "yakutsk"s};

    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo"s);
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mp"s);
    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;

    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "z");
    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;
    return 0;
}



// std::set<int>::const_iterator FindNearestElement(const std::set<int>& numbers, int border) {
//     using namespace std;

//     if(numbers.begin() != numbers.end()) 
//     {
//         auto up = numbers.upper_bound(border);
//         auto low = prev(up, 1);

//         if((up == numbers.end()) || (abs(border - *low) <= abs(border - *up))){
//             return low;
//         }
//         else {
//             return up;
//         }
//     } 
//     return numbers.end();
// }

// int main() {
//     using namespace std;
//     set<int> numbers = {1, 4, 6};
//     cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
//          << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
//          << *FindNearestElement(numbers, 100) << endl;
//     set<int> empty_set;
//     cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
//     return 0;
// }