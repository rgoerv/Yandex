#include <cstdint>
#include <iostream>
#include <optional>
#include <limits>

using namespace std;

template <typename T>
std::optional<T> SafeAdd(T lhs, T rhs) {
    if(((lhs < 0 && rhs < 0) && (lhs < std::numeric_limits<T>::min() - rhs)) 
        || ((lhs > 0 && rhs > 0) && (lhs > std::numeric_limits<T>::max() - rhs))) {
        return nullopt;
    }
    return lhs + rhs;
}

int main() {
    int64_t a;
    int64_t b;
    cin >> a >> b;
    const auto& result = SafeAdd(a, b);
    if(!result.has_value()) {
        cout << "Overflow!"s << endl;
    }
    else {
        cout << *result << endl;
    }
    
}