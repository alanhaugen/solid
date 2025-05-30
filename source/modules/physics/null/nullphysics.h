#ifndef NULLPHYSICS_H
#define NULLPHYSICS_H

#include <modules/physics/physics.h>

namespace Physics
{

class NullPhysics : public IPhysics
{
public:
    NullPhysics();
    bool Intersect(const Ray &r, const HitBox *hitbox);
    Collider *Collide(HitBox *hitbox, const char *type = "Box Collidable");
    HitBox *CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type);
    void RemoveHitBox(HitBox *hitbox);
    void Update();
    void Reset();
};

}

#endif // NULLPHYSICS_H
