#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "modules/renderer/light.h"
#include "core/components/component.h"

class DirectionalLight : public ILight, public IComponent
{
private:
    glm::vec3 direction;

public:
    DirectionalLight();
    DirectionalLight(glm::vec3 colour, float aIntensity, float dIntensity);

    void Update();
    void UpdateAfterPhysics();
};

#endif // DIRECTIONALLIGHT_H
