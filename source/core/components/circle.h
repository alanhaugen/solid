#ifndef CIRCLE_H
#define CIRCLE_H

#include <glm/vec3.hpp>

class Circle
{
public:
    Circle(int x, int y, float radius = 1.0f);
    Circle(glm::vec3 posIn3D, float radius = 1.0f);
};

#endif // CIRCLE_H
