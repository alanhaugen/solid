#include "nullphysics.h"

Physics::NullPhysics::NullPhysics()
{
}

//! There are a host of fast algorthims at https://www.realtimerendering.com/intersections.html
bool Physics::NullPhysics::Intersect(const Ray &ray, const HitBox *hitbox)
{
    return false;
}

Physics::IPhysics::Collider *Physics::NullPhysics::Collide(HitBox *hitbox, const char *type)
{
    return NULL;
}

Physics::IPhysics::HitBox *Physics::NullPhysics::CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type = "solid")
{
    return NULL;
}

void Physics::NullPhysics::Update()
{
}
