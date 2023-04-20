#include "sphere.h"

Sphere::Sphere()
{
    tag = "sphere";
}

Sphere::~Sphere()
{
    delete mesh;
}

Sphere::Sphere(glm::vec3 pos, float _radius)
{
    position = pos;
    radius = _radius;

    tag = "sphere";

    // TODO: icosphere or uv sphere? both?

    mesh = NULL;
}

void Sphere::Update()
{
    //mesh->SetPosition(position);
}

void Sphere::UpdateAfterPhysics()
{
}
