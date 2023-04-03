#include "carpenter.h"
#include "square_calculation.h"
#include "wall.h"

int Carpenter::CalcShelves(const Wall & wall) const {
    return CalcSquare(wall.GetHeight(), wall.GetWidth()) * 0.5;
}