#ifndef SPHERE_H
#define SPHERE_H

#include "core/components/component.h"
#include "core/components/mesh.h"

class Sphere : public IComponent
{
public:
    Sphere();
    ~Sphere();
    Sphere(glm::vec3 position, float radius);

    void Update();
    void UpdateAfterPhysics();

private:
    glm::vec3 position;
    float radius;

    Mesh *mesh;
};

#endif // SPHERE_H
