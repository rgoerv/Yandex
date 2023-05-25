#include <cassert>
#include <cstddef>  // нужно для nullptr_t
#include <algorithm>

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* value_ptr = nullptr;
public:
    UniquePtr() = default;
    explicit UniquePtr(T* ptr) {
        value_ptr = ptr;
    }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        if(this != &other) {
            value_ptr = other.Release();            
        }   
    }

    UniquePtr& operator=(nullptr_t) {
        delete value_ptr;
        value_ptr = nullptr;
        return *this;
    }
    UniquePtr& operator=(UniquePtr&& other) {
        if(this != &other) {
            T* ptr = other.Release();
            other.Reset(Release());
            Reset(ptr);
        }
        return *this;
    }
    ~UniquePtr() noexcept {
        delete value_ptr;
    }

    T& operator*() const {
        return *value_ptr;
    }
    T* operator->() const {
        return Get();
    }
    T* Release() {
        T* value = value_ptr;
        value_ptr = nullptr;
        return value;
    }
    void Reset(T* ptr) {
        delete value_ptr;
        value_ptr = ptr;
    }
    void Swap(UniquePtr& other) {
        T* ptr = other.Release();
        other.Reset(Release());
        Reset(ptr);
    }
    T* Get() const {
        return value_ptr;
    }
};

struct Item {
    static int counter;
    int value;
    Item(int v = 0)
        : value(v) 
    {
        ++counter;
    }
    Item(const Item& other)
        : value(other.value) 
    {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        ptr.Reset(new Item);
        assert(Item::counter == 1);
    }
    assert(Item::counter == 0);

    {
        UniquePtr<Item> ptr(new Item);
        assert(Item::counter == 1);

        auto rawPtr = ptr.Release();
        assert(Item::counter == 1);

        delete rawPtr;
        assert(Item::counter == 0);
    }
    assert(Item::counter == 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    assert(ptr.Get()->value == 42);
    assert((*ptr).value == 42);
    assert(ptr->value == 42);
}

int main() {
    TestLifetime();
    TestGetters();
}