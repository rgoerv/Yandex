#pragma once

namespace raii {

template <typename Provider>
class Booking {
private:
    using BookingId = typename Provider::BookingId;

    Provider* provider_;
    int counter;

public:
    Booking(Provider* p, int count) 
        : provider_(p)
        , counter(count)
    {
    }
    Booking(const Booking&) = delete;
    Booking(Booking&& other) = default;
    Booking& operator=(const Booking&) = delete;
    Booking& operator=(Booking&& other) = default;
   
    ~Booking() {
        if (provider_) {
            (*provider_).CancelOrComplete(*this);
        }
    }

    // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
    // BookingId GetId() const;   
};

}