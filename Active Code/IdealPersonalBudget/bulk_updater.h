#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    DayInfo delta = {};
};

struct BulkTaxApplier {
    BulkTaxApplier(double percent)
        : factor(1.0 - percent / 100.0)
    {
    }

    BulkTaxApplier() = default;

    double factor = 1.0;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta = add_.delta * other.tax_.factor + other.add_.delta;
    }

    DayInfo Collapse(DayInfo origin, IndexSegment segment) const {
        return origin * tax_.factor + DayInfo{ add_.delta.income * static_cast<double>(segment.length()), 
            add_.delta.spend * static_cast<double>(segment.length()) };
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};
