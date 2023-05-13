#ifndef BULLETPHYSICS_H
#define BULLETPHYSICS_H

#include "../physics.h"

//namespace Physics
//{

#include <core/x-platform/string.h>
#include <btBulletDynamicsCommon.h>

class BulletPhysics : public Physics::IPhysics
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
    bool Intersect(const Ray &r, const HitBox *hitbox);
    Collider *Collide(HitBox *hitbox, const char *type = "Box Collidable");
    HitBox *CreateHitBox(glm::vec3 dimensions, Mat *matrix, const char *type);
    void Update();
};

//}

#endif // NULLPHYSICS_H
