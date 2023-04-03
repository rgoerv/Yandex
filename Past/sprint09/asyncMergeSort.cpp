#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <functional>
#include <future>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

// Ускорьте с помощью параллельности
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end, int DEPTH = 0) {
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. Создаём вектор, содержащий все элементы текущего диапазона
    vector elements(range_begin, range_end);
    // Тип элементов — typename iterator_traits<RandomIt>::value_type

    // 3. Разбиваем вектор на две равные части
    auto mid = elements.begin() + range_length / 2;

    // 4. Вызываем функцию MergeSort от каждой половины вектора
    constexpr int MAX_ASYNC_DEPTH = 2;
    if(DEPTH <= MAX_ASYNC_DEPTH) {
        
        auto begin = elements.begin();
        auto par = async([&begin, &mid, &DEPTH](){ 
            MergeSort(begin, mid, ++DEPTH);
        });
        
        MergeSort(mid, elements.end(), ++DEPTH);
        par.get();
    }
    else {
        MergeSort(elements.begin(), mid, ++DEPTH);
        MergeSort(mid, elements.end(), ++DEPTH);
    }    

    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    mt19937 generator;

    vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);

    // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    shuffle(test_vector.begin(), test_vector.end(), generator);

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    // Проверяем, можно ли передать указатели
    MergeSort(test_vector.data(), test_vector.data() + test_vector.size());

    system("pause");
    return 0;
}