#ifndef NULLPHYSICS_H
#define NULLPHYSICS_H

#include "../physics.h"
#include <core/x-platform/string.h>

class NullPhysics : public IPhysics
{
private:
    Array<IPhysics::Collider*> colliders;

public:
    NullPhysics();
    virtual bool Intersect(const Ray &r, const HitBox *hitbox);
    virtual Collider *Collide(const char *type, HitBox *hitbox);
    virtual HitBox *CreateHitBox(glm::vec3 dimensions, const char *type);
    virtual void Update();
};

#endif // NULLPHYSICS_H
