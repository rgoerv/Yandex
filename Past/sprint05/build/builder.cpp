#include "square_calculation.h"
#include "wall.h"
#include "builder.h"

double Builder::CalcBricksNeeded(const Wall& wall) const {
    double height = wall.GetHeight();
    double width = wall.GetWidth();
    return CalcSquare(width, height) * 5;
}
