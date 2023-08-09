#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <algorithm>
#include <memory>

#include <iostream>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory& rhs) = delete;
    
    explicit RawMemory(size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) // 
    {
    }

    RawMemory(RawMemory&& other) noexcept
        : buffer_(std::exchange(other.buffer_, nullptr))
        , capacity_(std::exchange(other.capacity_, 0)) //
    {
    }

    ~RawMemory() {
        Deallocate(buffer_);
    }

    RawMemory& operator=(RawMemory&& rhs) noexcept {
        if(this != &rhs) {
            buffer_ = std::exchange(rhs.buffer_, nullptr);
            capacity_ = std::exchange(rhs.capacity_, 0);
        }
        return *this;
    }

    T* operator+(size_t offset) noexcept {
        // Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
        assert(offset <= capacity_);
        return buffer_ + offset;
    }

    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }

    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }

    const T* GetAddress() const noexcept {
        return buffer_;
    }

    T* GetAddress() noexcept {
        return buffer_;
    }

    size_t Capacity() const {
        return capacity_;
    }

private:
    // Выделяет сырую память под n элементов и возвращает указатель на неё
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }

    // Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
    Vector() = default;

    explicit Vector(size_t size)
        : data_(size)
        , size_(size) //
    {
        std::uninitialized_value_construct_n(data_.GetAddress(), size);
    }

    Vector(const Vector& other)
        : data_(other.size_)
        , size_(other.size_)  //
    {
        std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
    }

    Vector(Vector&& other) noexcept 
        : data_(std::move(other.data_))
        , size_(std::exchange(other.size_, 0)) // 
    {  
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (data_.Capacity() < rhs.size_) {
                Vector rhs_copy(rhs);
                Swap(rhs_copy);
            } else {
                if(rhs.size_ < size_) {
                    std::copy_n(rhs.data_.GetAddress(), rhs.size_, data_.GetAddress());
                    std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
                }
                else {
                    std::copy_n(rhs.data_.GetAddress(), size_, data_.GetAddress());
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, data_.GetAddress() + size_);   
                }           
                size_ = rhs.size_;
            }
        }
        return *this;
        
    }

    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            Swap(rhs);
        }
        return *this;
    }

    using iterator = T*;
    using const_iterator = const T*;
    
    iterator begin() noexcept {
        return data_.GetAddress();
    }

    iterator end() noexcept {
        return data_.GetAddress() + size_;
    }

    const_iterator begin() const noexcept {
        return data_.GetAddress();
    }

    const_iterator end() const noexcept {
        return data_.GetAddress() + size_;
    }

    const_iterator cbegin() const noexcept {
        return data_.GetAddress();
    }

    const_iterator cend() const noexcept {
        return data_.GetAddress() + size_;
    }

    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        assert((pos >= cbegin()) && (pos <= cend()));
        auto dst = std::distance(cbegin(), pos);

        if(size_ >= data_.Capacity()) {
            RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
            new (new_data.GetAddress() + dst) T(std::forward<Args>(args)...);
            try{
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress(), dst, new_data.GetAddress());
                } else {
                    std::uninitialized_copy_n(data_.GetAddress(), dst, new_data.GetAddress());
                }
            } catch(...) {
                std::destroy_at(new_data.GetAddress() + dst);
                throw;
            }
            try {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    std::uninitialized_move_n(data_.GetAddress() + dst, size_ - dst, new_data.GetAddress() + dst + 1);
                } else {
                    std::uninitialized_copy_n(data_.GetAddress() + dst, size_ - dst, new_data.GetAddress() + dst + 1);
                }
            } catch(...) {
                std::destroy_n(new_data.GetAddress(), dst);
                throw;
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        }
        else {
            if(pos != cend()) {
                // временный обьект со значением(уберегаем от перезаписывания, если элемент того же вектора)
                T copy(std::forward<Args>(args)...);
                // cдвигаем последный элемент верктора вправо в неинициализированную область
                new (data_.GetAddress() + size_) T(std::move(data_[size_ - 1]));
                // перемещаем нужные элементы вправо на 1
                std::move_backward(data_.GetAddress() + dst, data_.GetAddress() + size_ - 1, data_.GetAddress() + size_);
                // вставляем элемент на свое место
                data_[dst] = std::forward<T>(copy);
            }
            else {
                new (data_.GetAddress() + size_) T(std::forward<Args>(args)...);
            }
        }
        ++size_;
        return iterator{data_.GetAddress() + dst};
    }

    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }
    
    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::forward<T>(value));
    }

    iterator Erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>) {
        auto dst = std::distance(cbegin(), pos);
        // удаляем элемент на этой позиции
        std::destroy_at(data_.GetAddress() + dst);
        // перемещаем элементы следующие за удаленным влево на 1
        std::move(data_.GetAddress() + dst + 1, data_.GetAddress() + size_, data_.GetAddress() + dst);
        --size_;
        // возвращаем итератор на новый элемент удаленной позиции
        return iterator{data_.GetAddress() + dst};
    }

    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }
    
    void Swap(Vector& other) noexcept {
        if(this != &other) {
            data_.Swap(other.data_);
            std::swap(size_, other.size_);
        }
    }

    void Resize(size_t new_size) {
        if(size_ < new_size) {
            if(data_.Capacity() < new_size) {
                Reserve(new_size);
            }
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
        }
        else {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
        }
        size_ = new_size;
    }

    template <typename Forward>
    void PushBack(Forward&& value) {
        if(size_ >= data_.Capacity()) {
            
            RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);

            new (new_data.GetAddress() + size_) T(std::forward<Forward>(value));

            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        }
        else {
            new (data_.GetAddress() + size_) T(std::forward<Forward>(value));
        }
        ++size_;
    }
    
    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        if(size_ >= data_.Capacity()) {
            
            RawMemory<T> new_data(size_ == 0 ? 1 : size_ * 2);
            new (new_data.GetAddress() + size_) T(std::forward<Args>(args)...);

            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        }
        else {
            new (data_.GetAddress() + size_) T(std::forward<Args>(args)...);
        }
        ++size_;
        
        return data_[size_ - 1];
    }

    void PopBack() noexcept {
        assert(size_ > 0);
        std::destroy_at(data_.GetAddress() + (--size_));
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_data(new_capacity);

        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        } else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }

        std::destroy_n(data_.GetAddress(), size_);
        data_.Swap(new_data);
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;
};