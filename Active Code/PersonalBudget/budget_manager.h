#pragma once
#include "date.h"
#include "parser.h"

#include <vector>

namespace Manager {

struct DayInfo {
    double income = .0;
    double spend = .0;
};

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};
    const int max_days = Date::ComputeDistance(START_DATE, END_DATE);

    BudgetManager();
    
    void ComputeIncome(TimePeriod period) const;
    void Earn(TimePeriod period, double income);
    void PayTax(TimePeriod period, double rate);
    void Spend(TimePeriod period, double profit);

private:
    std::vector<DayInfo> DaysInformation;
};

} // namespace Menager