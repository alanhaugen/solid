#include "rectangle.h"

Rectangle::Rectangle(const int red, const int green, const int blue, const int x1, const int y1, const int x2, const int y2)
    : Sprite(red, green, blue, x1, y1)
{
    width  = x2 - x1;
    height = y2 - y1;
}
