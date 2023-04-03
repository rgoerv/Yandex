#pragma once
#include "square_calculation.h"
#include "wall.h"

class Builder {
public:
    double CalcBricksNeeded(const Wall& wall) const;
};