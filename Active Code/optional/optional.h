#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value) {
        element = new(&data_[0]) T(value);
        is_initialized_ = true;
    }
    Optional(T&& value) {
        element = new(&data_[0]) T(std::forward<T>(value));
        is_initialized_ = true;
    }
    Optional(const Optional& other) {
        if(other.HasValue()) {
            is_initialized_ = true;
            element = new(&data_[0]) T(other.Value());
        }
    }
    Optional(Optional&& other) {
        if(other.HasValue()) {
            is_initialized_ = other.HasValue();
            element = new(&data_[0]) T(std::forward<T>(other.Value()));
        }
    }

    Optional& operator=(const T& value) {
        if(is_initialized_) {
            *element = value;
        }
        else {
            element = new(&data_[0]) T(value);
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(T&& rhs) {
        if(is_initialized_) {
            *element = std::forward<T>(rhs);
        }
        else {
            element = new(&data_[0]) T(std::forward<T>(rhs));
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(const Optional& rhs){
        if(is_initialized_ && rhs.HasValue()) {
            Value() = rhs.Value();
            is_initialized_ = true;
        }
        else if(is_initialized_ && !rhs.HasValue()) {
            Reset();
        }
        else if(!is_initialized_ && rhs.HasValue()) {
            element = new(&data_[0]) T(rhs.Value());
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(Optional&& rhs) {
        if(is_initialized_ && rhs.HasValue()) {
            Value() = std::forward<T>(rhs.Value());
            is_initialized_ = true;
        }
        else if(is_initialized_ && !rhs.HasValue()) {
            Reset();
        }
        else if(!is_initialized_ && rhs.HasValue()) {
            element = new(&data_[0]) T(std::forward<T>(rhs.Value()));
            is_initialized_ = true;
        }
        return *this;
    }

    ~Optional() {
        if(is_initialized_) {
            element->~T();
        }   
    }

    bool HasValue() const {
        return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*(){
        return *element;
    }
    const T& operator*() const {
        return *element;
    }
    T* operator->() {
        return element;
    }
    const T* operator->() const {
        return element;
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
        if(is_initialized_) {
            return *element;
        }
        throw BadOptionalAccess();
    }
    const T& Value() const {
        if(is_initialized_) {
            return *element;
        }
        throw BadOptionalAccess();
    }

    void Reset() {
        if(is_initialized_) {
            element->~T();
        }
        is_initialized_ = false;
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* element = nullptr;
};