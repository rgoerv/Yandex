#include <cassert>
#include <vector>
#include <iostream>

template <typename T>
void ReverseArray(T* start, size_t size) {
    using namespace std;
    if(size) {
        size_t iterations = (size / 2);
        --size;
        for(size_t i = 0; i < iterations; ++i) {
            T tmp = start[size - i];
            start[size - i] = start[i];
            start[i] = tmp; 
        }
    }    
}

int main() {
    using namespace std;
    
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{9, 8, 7, 6, 5, 4, 3, 2, 1}));
}