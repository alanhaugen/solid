#ifndef BOUNCEPHYSICS_H
#define BOUNCEPHYSICS_H

#include <modules/physics/null/nullphysics.h>
#include <core/x-platform/ptr.h>

namespace Physics
{

#include <core/x-platform/string.h>

class BouncePhysics : public NullPhysics
{
protected:
    Array<Ptr<IPhysics::Collider* > > colliders;

public:
    BouncePhysics();
    bool Intersect(const Ray &r, const HitBox *hitbox);
    Collider *Collide(HitBox *hitbox = NULL, const char *type = "solid");
    HitBox *CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type);
    void Update();
};

}

#endif // BOUNCEPHYSICS_H
