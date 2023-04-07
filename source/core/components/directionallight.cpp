#include "directionallight.h"

DirectionalLight::DirectionalLight()
{
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

DirectionalLight::DirectionalLight(glm::vec3 _colour, float aIntensity, float dIntensity)
{
    colour = _colour;
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

void DirectionalLight::Update()
{
}

void DirectionalLight::UpdateAfterPhysics()
{
}
