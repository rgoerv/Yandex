#include <stack>
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
class Queue {
public:
    void Push(const Type& element) {
        // напишите реализацию
        Stack.push(element);
    }
    void Pop() {
        // напишите реализацию
        // for(size_t i = 0; !stack_second.empty(); ++i){
        //     stack_second.pop();
        // }
        // cout << "Stack input :" << endl;
        for(size_t i = 0; !Stack.empty(); ++i){
            if(Stack.size() == 1){
                Stack.pop();
                break;
            }
            stack_second.push(Stack.top());
            // cout << Stack.top();
            Stack.pop();
        }
        // cout << endl << "Stack output :" << endl;
        
//        Stack = stack_second;
        for(size_t i = 0; !stack_second.empty(); ++i){
            if(stack_second.size() == 1){
                break;
            }
            Stack.push(stack_second.top());
            // cout << stack_second.top();
            stack_second.pop();
        }
        // cout << endl;
    }
    Type& Front() {
        // напишите реализацию
        stack_second = Stack;
        for(size_t i = 0; !stack_second.empty(); ++i){
            if(stack_second.size() == 1){
                break;
            }
            // cout << stack_second.top();
            stack_second.pop();
        }
        return stack_second.top();
    }
    uint64_t Size() const {
        // напишите реализацию
        return Stack.size();
    }
    bool IsEmpty() const {
        // напишите реализацию
        return Stack.empty();
    }

private:
    stack<Type> Stack;
    stack<Type> stack_second;
};

int main() {
    Queue<int> queue;
    vector<int> values(5);
    // заполняем вектор для тестирования очереди
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());
    PrintRange(values.begin(), values.end());
    cout << "Заполняем очередь"s << endl;
    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }
    cout << "Вынимаем элементы из очереди"s << endl;
    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}