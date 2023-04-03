#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int n, F drop) {
    // Переделайте этот алгоритм, имеющий линейную сложность.
    // В итоге должен получится логарифмический алгоритм.
    int first = 0;
    int last = n;
    while (first != last){
        int median = (first + last) / 2;
        if(drop(median)){
            last = median;
        }
        else {
            first = median + 1;
        }
    }
    return first;
}

int main() {
    int n,t;
    cout << "Enter n and target floor number: "s << endl;
    cin >> n >> t;

    int count = 0;
    int found = FindFloor(n, [t, &count](int f) {
        ++count;
        return f >= t;
    });

    cout << "Found floor "s << found << " after "s << count << " drops"s;

    return 0;
}