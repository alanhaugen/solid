#ifndef BULLETPHYSICS_H
#define BULLETPHYSICS_H

#include "../physics.h"
#include <core/x-platform/string.h>
#include <btBulletDynamicsCommon.h>

class BulletPhysics : public IPhysics
{
private:
    Array<IPhysics::Collider*> colliders;

    //collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration;

    //use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher;

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicsWorld;

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    //create a few basic rigid bodies
    int i;

public:
    BulletPhysics();
    ~BulletPhysics();
    virtual bool Intersect(const Ray &r, const HitBox *hitbox);
    virtual Collider *Collide(const char *type, HitBox *hitbox);
    virtual HitBox *CreateHitBox(glm::vec3 dimensions, const char *type);
    virtual void Update();
};

#endif // NULLPHYSICS_H
