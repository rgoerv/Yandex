#include <stack>
#include <numeric>
#include <vector>
#include <iostream>

using namespace std;

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        if (elements_.empty()) {
            elements_.push(element);
            ++push_count;
            return;
        }

        Type last_elem = elements_.top();
        ++if_count;
        if (last_elem < element) {
            elements_.pop();
            Push(element);

            elements_.push(last_elem);
            ++push_count;
        } else {
            elements_.push(element);
            ++push_count;
        }
    }

    // остальные функции не понадобятся
    size_t if_count = 0;
    size_t push_count = 0;
private:
    stack<Type> elements_;
};

int main(){
    vector<int> numbers(100);
    iota(numbers.begin(), numbers.end(), 1);
    SortedStack<int> stacks;
    for(auto it = numbers.begin(); it != numbers.end(); ++it){
        stacks.Push(*it);
    }
    cout << stacks.push_count << endl;
    cout << stacks.if_count << endl;
}