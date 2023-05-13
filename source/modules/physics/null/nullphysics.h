#ifndef NULLPHYSICS_H
#define NULLPHYSICS_H

#include "../physics.h"

namespace Physics
{

#include <core/x-platform/string.h>

class NullPhysics : public IPhysics
{
protected:
    Array<IPhysics::Collider*> colliders;

public:
    NullPhysics();
    bool Intersect(const Ray &r, const HitBox *hitbox);
    Collider *Collide(HitBox *hitbox, const char *type = "Box Collidable");
    HitBox *CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type);
    void Update();
};

}

#endif // NULLPHYSICS_H
