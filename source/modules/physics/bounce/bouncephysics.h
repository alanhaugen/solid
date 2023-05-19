#ifndef BOUNCEPHYSICS_H
#define BOUNCEPHYSICS_H

#include "../physics.h"
#include <core/x-platform/ptr.h>

namespace Physics
{

#include <core/x-platform/string.h>

class BouncePhysics : public IPhysics
{
protected:
    Array<Ptr<IPhysics::Collider* > > colliders;

public:
    BouncePhysics();
    bool Intersect(const Ray &r, const HitBox *hitbox);
    Collider *Collide(HitBox *hitbox, const char *type = "Box Collidable");
    HitBox *CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type);
    void Update();
};

}

#endif // BOUNCEPHYSICS_H
