#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>

class Trip {
public:
    
    Trip() = delete;

    Trip(HotelProvider& hp, FlightProvider& fp)
        : HP(hp)
        , FP(fp)
    {
    }

    Trip(Trip&&) = default;
    Trip& operator=(Trip&&) = default;

    Trip(const Trip&) = delete;
    Trip& operator=(const Trip&) = delete;

    void Cancel() {
        // отменяем бронирования у провайдеров
        for (auto& id : hotels) {
            HP.Cancel(id);
        }
        // чистим вектора в структуре trip
        hotels.clear();
        for (auto& id : flights) {
            FP.Cancel(id);
        }
        flights.clear();
    }

    ~Trip() {
        Cancel();
    }

    std::vector<HotelProvider::BookingId> hotels;
    std::vector<FlightProvider::BookingId> flights;

private:

    HotelProvider& HP;
    FlightProvider& FP;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);
        {
            FlightProvider::BookingData flight_booking_data;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        {
            HotelProvider::BookingData hotel_booking_data;
            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
        }
        {
            FlightProvider::BookingData flight_booking_data;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};