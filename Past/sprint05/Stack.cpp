// template <typename Type>
// class StackMin {
// public:
//     void Push(const Type& element) {
//     // напишите реализацию метода
//         elements.Push(element);
//         min_elements.push_back(element);
//     }
//     void Pop() {
//     // напишите реализацию метода
//         min_elements.pop_back();
//         elements.Pop();
//     }
//     const Type& Peek() const {
//         return elements.Peek();
//     }
//     Type& Peek() {
//         return elements.Peek();
//     }
//     void Print() const {
//         // работу этого метода мы проверять не будем,
//         // но если он вам нужен, то можете его реализовать
//         elements.Print();
//     }
//     uint64_t Size() const {
//         return elements.size();
//     }
//     bool IsEmpty() const {
//         return elements.IsEmpty();
//     }
//     const Type& PeekMin() const {
//     // напишите реализацию метода
//         return *min_element(min_elements.begin(), min_elements.end());
//     }
//     Type& PeekMin() {
//     // напишите реализацию метода
//         return *min_element(min_elements.begin(), min_elements.end());
//     }
// private:
//     Stack<Type> elements;
//     // возможно, здесь вам понадобится что-то изменить
//     vector<Type> min_elements;
// };

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <utility>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        // вставка элементов в стек
        elements.push_back(element);
    }
    void Pop() {
        // удаление элемента из стека
        elements.pop_back();
    }
    const Type& Peek() const {
        // получения значения вершины стека
        return elements.back();
    }
    Type& Peek() {
        // получения значения вершины стека
        return elements.back();
    }
    void Print() const {
        PrintRange(elements.begin(), elements.end());
    }
    uint64_t Size() const {
        return elements.size();
    }
    bool IsEmpty() const {
        return elements.empty();        
    }

private:
    vector<Type> elements;
};

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        // напишите реализацию метода
        if(elements_.IsEmpty() || (element <= elements_.Peek())) {
            elements_.Push(element);
            return;
        }
        Type last = elements_.Peek();
        elements_.Pop();
        Push(element);
        elements_.Push(last);
    }
    void Pop() {
        // напишите реализацию метода
        elements_.Pop();        
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
private:
    Stack<Type> elements_;
};


int main() {
    SortedStack<int> stack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    
    random_shuffle(values.begin(), values.end());
    // заполняем стек и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        stack.Push(values[i]);
        stack.Print();
    }
}