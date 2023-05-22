#include <cassert>
#include <functional>
#include <string>
#include <mutex>
#include <optional>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init) 
        : init_function(init)
    {
    }

    bool HasValue() const {
        return value != nullopt;
    }

    const T& Get() const {
        std::lock_guard lock(value_guard);
        if(!HasValue()) {
            value = init_function();
        }
        return *value;
    }

private:
    function<T()> init_function;
    mutable std::optional<T> value;
    mutable mutex value_guard;
};

void UseExample() {
    const string big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
    });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}