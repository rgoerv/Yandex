#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    // место для вашего решения
    if(static_cast<int64_t>(v.size())*(i + 1)/(n + 1) < log2(v.size())){
        cout << "Using find_if"s << endl;
        return distance(v.begin(), find_if(v.begin(), v.end(), [i](const int element){
            return element > i;
        }));
    }
    else {
        cout << "Using upper_bound"s << endl;
        return distance(v.begin(), upper_bound(v.begin(), v.end(), i));
    }
}

int main() {
    static const int NUMBERS = 10'000; //1'000'000;
    static const int MAX = 4'999; //1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}