#include "painter.h"
#include "wall.h"

void Painter::Paint(Wall& wall, const Wall::Color color) {
    wall.SetColor(color);
}
