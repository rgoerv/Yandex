#pragma once


struct DayInfo {
    double income = 0.0;
    double spend = 0.0;

    double Profit() const {
        return income - spend;
    }
    
    DayInfo operator+(const DayInfo& other) const {
        return DayInfo{ income + other.income, spend + other.spend };
    }

    DayInfo operator+(double other) const {
        return DayInfo{ income + other, spend + other };
    }

    DayInfo operator*(const DayInfo& other) const {
        return DayInfo{ income * other.income, spend };
    }

    DayInfo operator*(double other) const {
        return DayInfo{ income * other, spend };
    }

    DayInfo& operator+=(const DayInfo& other) {
        income += other.income;
        spend += other.spend;
        return *this;
    }

};