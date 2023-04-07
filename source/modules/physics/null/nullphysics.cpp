#include "nullphysics.h"

NullPhysics::NullPhysics()
{
}

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

//! There are a host of fast algorthims at https://www.realtimerendering.com/intersections.html
bool NullPhysics::Intersect(const Ray &ray, const HitBox *hitbox)
{
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // ray.orig is origin of ray
    float lbx = hitbox->position->x - hitbox->dimensions.x;
    float rtx = hitbox->position->x + hitbox->dimensions.x;
    float lby = hitbox->position->y - hitbox->dimensions.y;
    float rty = hitbox->position->y + hitbox->dimensions.y;
    float lbz = hitbox->position->z + hitbox->dimensions.z;
    float rtz = hitbox->position->z - hitbox->dimensions.z;

    float t1 = (lbx - ray.origin.x) * ray.invdir.x;
    float t2 = (rtx - ray.origin.x) * ray.invdir.x;
    float t3 = (lby - ray.origin.y) * ray.invdir.y;
    float t4 = (rty - ray.origin.y) * ray.invdir.y;
    float t5 = (lbz - ray.origin.z) * ray.invdir.z;
    float t6 = (rtz - ray.origin.z) * ray.invdir.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        //t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        //t = tmax;
        return false;
    }

    //t = tmin;
    return true;
}

IPhysics::Collider *NullPhysics::Collide(const char *type, HitBox *hitbox)
{
    for (unsigned int i = 0; i < colliders.Size(); i++)
    {
        for (unsigned int k = 0; k < colliders[i]->collisions.Size(); k++)
        {
            if (colliders[i]->collisions[k] == hitbox && colliders[i]->hitbox->type == type)
            {
                colliders[i]->direction = glm::vec3();
                return colliders[i];
            }
        }
    }

    return NULL;
}

IPhysics::HitBox *NullPhysics::CreateHitBox(glm::vec3 dimensions, const char *type = "solid")
{
    HitBox *hitbox = new HitBox;
    hitbox->dimensions = dimensions;
    hitbox->type = type;

    Collider *collider = new Collider;
    collider->hitbox = hitbox;

    colliders.Add(collider);

    return hitbox;
}

void NullPhysics::Update()
{
    for (unsigned int i = 0; i < colliders.Size(); i++)
    {
        colliders[i]->collisions.Clear();

        for (unsigned int k = 0; k < colliders.Size(); k++)
        {
            // Don't detect collisions against oneself
            if (i == k)
            {
                continue;
            }

            // Collision detection (wait.. why not just check against the hitboxes???)
            /*HitBox *hitbox1 = colliders[i]->hitbox;
            HitBox *hitbox2 = colliders[k]->hitbox;

            if (hitbox1->position->x - hitbox1->dimensions.x < hitbox2->position->x + hitbox2->position->x &&
                obj1.y > obj2.y - hitbox2.y &&
                obj1.x > obj2.x + hitbox2.x &&
                obj1.y < obj2.y + hitbox2.y)
            {
                colliders[i]->collisions.Add(colliders[k]->hitbox);
            }*/
        }
    }
}
