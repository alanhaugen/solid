#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "core/components/component.h"
#include "modules/renderer/light.h"

class PointLight : public ILight, public IComponent
{
protected:
    float constant, linear, exponent;

public:
    glm::vec3 position;

    PointLight();
    PointLight(glm::vec3 pos, glm::vec3 colour_ = glm::vec3(1.0));

    void Update();
    void UpdateAfterPhysics();
};

#endif // POINTLIGHT_H
