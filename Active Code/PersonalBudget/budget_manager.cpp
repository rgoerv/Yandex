#include "budget_manager.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace Manager {

BudgetManager::BudgetManager() {
    DaysInformation.resize(max_days);
}

void BudgetManager::ComputeIncome(TimePeriod period) const {
    int count_day = Date::ComputeDistance(period.begin, period.end) + 1;
    int start_day = Date::ComputeDistance(START_DATE, period.begin);

    double income = std::accumulate(DaysInformation.begin() + start_day, DaysInformation.begin() + start_day + count_day, .0,
        [](const double& acc, const DayInfo& value) {
            return acc + (value.income - value.spend);
        });
    std::cout << std::setprecision(6) << income << std::endl;
}

void BudgetManager::Earn(TimePeriod period, double income) {
    int count_day = Date::ComputeDistance(period.begin, period.end) + 1;
    double income_in_day = income / count_day;
    int start_day = Date::ComputeDistance(START_DATE, period.begin);

    std::for_each_n(DaysInformation.begin() + start_day, count_day,
        [income_in_day](DayInfo& day) {
            day.income += income_in_day;
        });
    }

void BudgetManager::PayTax(TimePeriod period, double rate) {
    int count_day = Date::ComputeDistance(period.begin, period.end) + 1;
    int start_day = Date::ComputeDistance(START_DATE, period.begin);

    std::for_each_n(DaysInformation.begin() + start_day, count_day,
        [rate](DayInfo& day) {
            day.income *= ((100.0 - rate) / 100.0);
        });
}

void BudgetManager::Spend(TimePeriod period, double spend) {
    int count_day = Date::ComputeDistance(period.begin, period.end) + 1;
    int start_day = Date::ComputeDistance(START_DATE, period.begin);

    double spend_in_day = spend / count_day;

    std::for_each_n(DaysInformation.begin() + start_day, count_day,
        [spend_in_day](DayInfo& day) {
            day.spend += spend_in_day;
        });
}

}