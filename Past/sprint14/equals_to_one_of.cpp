#include <cassert>
#include <string>
#include <string_view>

using namespace std;


template<typename T0, typename... Types>
bool EqualsToOneOf(const T0& Value0, const Types&... Values) {
    return ((Value0 == Values) || ...);
}

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}