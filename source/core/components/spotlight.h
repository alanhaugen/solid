#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "pointlight.h"

class SpotLight : public PointLight
{
private:
    glm::vec3 direction;
    float edge;

public:
    SpotLight(
            glm::vec3 colour,
            float ambientIntensity,
            float diffuseIntensity,
            glm::vec3 direction,
            glm::vec3 position,
            glm::vec3 quadricEquation,
            float edge);
};

#endif // SPOTLIGHT_H
