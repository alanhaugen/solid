#include "pointlight.h"

PointLight::PointLight()
{
    position = glm::vec3(0.0f);
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 colour_)
{
    position = pos;
    colour = colour_;
}

void PointLight::Update(float deltaTime)
{

}

void PointLight::UpdateAfterPhysics()
{

}
