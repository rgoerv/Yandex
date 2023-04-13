#include "libstat.h"

using namespace std;

namespace statistics {
namespace aggregations {
void Sum::PutValue(double value) {
    sum_ += value;
}

optional<double> Sum::Get() const {
    return sum_;
}

void Max::PutValue(double value) {
    cur_max_ = max(value, cur_max_.value_or(value));
}

optional<double> Max::Get() const {
    return cur_max_;
}

void Mean::PutValue(double value) {
    sum_.PutValue(value);
    ++count_;
}

optional<double> Mean::Get() const {
    auto val = sum_.Get();
    if (!val || count_ == 0) {
        return nullopt;
    }

    return *val / count_;
}

void StandardDeviation::PutValue(double value) {
    sum_.PutValue(value);
    sum_sq_.PutValue(value * value);
    ++count_;
}

optional<double> StandardDeviation::Get() const {
    auto val = sum_.Get();
    auto val2 = sum_sq_.Get();

    if (!val || !val2 || count_ < 2) {
        return nullopt;
    }

    return ::std::sqrt((*val2 - *val * *val / count_) / count_);
}

void Mode::PutValue(double value) {
    const size_t new_count = ++counts_[round(value)];

    if (new_count > cur_count_) {
        cur_max_ = value;
        cur_count_ = new_count;
    }
}

optional<double> Mode::Get() const {
    return cur_max_;
}
}
} // namespace statistics