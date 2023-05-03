#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "core/components/sprite.h"

class Rectangle : public Sprite
{
public:
    Rectangle(const int red = 0,
              const int green = 255,
              const int blue = 0,
              const int x1 = 0,
              const int y1 = 0,
              const int x2 = 0,
              const int y2 = 0);
};

#endif // RECTANGLE_H
