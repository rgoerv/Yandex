#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

template<typename T>
void PrintRange(T begin, T end)
{
    if(begin != end)
    {
        for(auto b = begin; b != end; ++b) {
            cout << *b << " ";
        }
        cout << endl;
    }
}

template <class RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {

    vector tmp(range_begin, range_end);

    auto begin = tmp.begin();
    auto end = tmp.end();
    int middle = distance(range_begin, range_end) / 2;

    if(begin - end <= -2) {
        MergeSort(begin, begin + middle);
        MergeSort(begin + middle, end);
    }
    merge(begin, begin + middle, begin + middle, end, range_begin);
}

int main() {
    vector<int> test_vector(10);

    iota(test_vector.begin(), test_vector.end(), 1);
    random_shuffle(test_vector.begin(), test_vector.end());
    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());
    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());
    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());
    return 0;
}