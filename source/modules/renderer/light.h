#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

class ILight
{
public:
    glm::vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

#endif // LIGHT_H
