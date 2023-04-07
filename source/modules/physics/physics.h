#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <core/containers/array.h>

class IPhysics
{
public:
    struct HitBox
    {
        const char *type;
        glm::vec3 dimensions;
        glm::vec3 *position;
    };

    struct Collider
    {
        Array<HitBox*> collisions;
        glm::vec3 direction;
        HitBox *hitbox;
    };

    class Ray
    {
    public:
        Ray(const glm::vec3 &orig, const glm::vec3 &dir) : origin(orig), dir(dir)
        {
            invdir.x = 1.0f / dir.x;
            invdir.y = 1.0f / dir.y;
            invdir.z = 1.0f / dir.z;
            sign[0] = (invdir.x < 0);
            sign[1] = (invdir.y < 0);
            sign[2] = (invdir.z < 0);
        }
        glm::vec3 origin, dir;       // ray orig and dir
        glm::vec3 invdir;
        int sign[3];
    };


    virtual ~IPhysics() {}
    virtual Collider *Collide(const char *type, HitBox *hitbox) = 0;
    virtual bool Intersect(const Ray &ray, const HitBox *hitbox) = 0;
    virtual HitBox *CreateHitBox(glm::vec3 dimensions, const char *type = "solid") = 0;
    virtual void Update() = 0;
};

#endif
