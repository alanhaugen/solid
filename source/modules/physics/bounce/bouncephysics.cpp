#include "bouncephysics.h"

Physics::BouncePhysics::BouncePhysics()
{
}

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

//! There are a host of fast algorthims at https://www.realtimerendering.com/intersections.html
bool Physics::BouncePhysics::Intersect(const Ray &ray, const HitBox *hitbox)
{
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // ray.orig is origin of ray
    float lbx = hitbox->position.x - hitbox->dimensions.x;
    float rtx = hitbox->position.x + hitbox->dimensions.x;
    float lby = hitbox->position.y - hitbox->dimensions.y;
    float rty = hitbox->position.y + hitbox->dimensions.y;
    float lbz = hitbox->position.z + hitbox->dimensions.z;
    float rtz = hitbox->position.z - hitbox->dimensions.z;

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

Physics::IPhysics::Collider *Physics::BouncePhysics::Collide(HitBox *hitbox, const char *type)
{
    if (hitbox == NULL)
    {
        if (colliders.Empty() == false)
        {
            for (unsigned int i = 0; i < colliders.Size(); i++)
            {
                if ((*colliders[i])->collisions.Empty() == false)
                {
                    for (unsigned int k = 0; k < (*colliders[i])->collisions.Size(); k++)
                    {
                        //if ((*colliders[i])->collisions[k]->type == type)
                        //{
                            return *colliders[i];
                        //}
                    }
                }
            }
        }

        return NULL;
    }

    for (unsigned int i = 0; i < colliders.Size(); i++)
    {
        for (unsigned int k = 0; k < (*colliders[i])->collisions.Size(); k++)
        {
            if ((*colliders[i])->collisions[k] == hitbox && (*colliders[i])->hitbox->type == type)
            {
                (*colliders[i])->direction = hitbox->oldPosition - hitbox->position;
                return *colliders[i];
            }
        }
    }

    return NULL;
}

Physics::IPhysics::HitBox *Physics::BouncePhysics::CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type = "solid")
{
    HitBox* hitbox = new HitBox;
    hitbox->dimensions = dimensions;
    hitbox->type = type;
    hitbox->active = true;
    hitbox->position = matrix->position;
    hitbox->oldPosition = hitbox->position;

    Ptr<Collider *> collider = new Collider;
    (*collider)->hitbox = hitbox;

    colliders.Add(collider);

    return hitbox;
}

void Physics::BouncePhysics::Update()
{
    for (unsigned int i = 0; i < colliders.Size(); i++)
    {
        (*colliders[i])->collisions.Clear();

        for (unsigned int k = 0; k < colliders.Size(); k++)
        {
            // Don't detect collisions against oneself
            if ((*colliders[i]) == (*colliders[k]))
            {
                continue;
            }

            if ((*colliders[i]) == (*colliders[k]))
            {
                continue;
            }

            // Collision detection
            HitBox *hitbox1 = (*colliders[i])->hitbox;
            HitBox *hitbox2 = (*colliders[k])->hitbox;

            if (hitbox1->active == false || hitbox2->active == false)
            {
                continue;
            }

            float x1 = hitbox1->position.x + hitbox1->dimensions.x; // first
            float x2 = hitbox1->position.x - hitbox1->dimensions.x;
            float y1 = hitbox1->position.y + hitbox1->dimensions.y;
            float y2 = hitbox1->position.y - hitbox1->dimensions.y;
            float z1 = hitbox1->position.z + hitbox1->dimensions.z;
            float z2 = hitbox1->position.z - hitbox1->dimensions.z;

            float x1_ = hitbox2->position.x - hitbox2->dimensions.x; // other
            float x2_ = hitbox2->position.x + hitbox2->dimensions.x;
            float y1_ = hitbox2->position.y - hitbox2->dimensions.y;
            float y2_ = hitbox2->position.y + hitbox2->dimensions.y;
            float z1_ = hitbox2->position.z - hitbox2->dimensions.z;
            float z2_ = hitbox2->position.z + hitbox2->dimensions.z;

            if (
                 x1 > x1_ &&
                 x2 < x2_ &&
                 y1 > y1_ &&
                 y2 < y2_ &&
                 z1 > z1_ &&
                 z2 < z2_
                )
            {
                (*colliders[i])->collisions.Add((*colliders[k])->hitbox);
            }
        }
    }

    // Update old position
    for (unsigned int i = 0; i < colliders.Size(); i++)
    {
        (*colliders[i])->hitbox->oldPosition = (*colliders[i])->hitbox->position;
    }
}

void Physics::BouncePhysics::Reset()
{
    colliders.Clear();
}
