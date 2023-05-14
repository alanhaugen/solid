#ifndef BULLETPHYSICS_H
#define BULLETPHYSICS_H

#include "../null/nullphysics.h"

//namespace Physics
//{

#include <core/x-platform/string.h>
#include <btBulletDynamicsCommon.h>

class BulletPhysics : public Physics::NullPhysics
{
private:
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

    void CreateBox(float x, float y, float z, float sizex, float sizey, float sizez, float mass_ = 0);

public:
    BulletPhysics();
    ~BulletPhysics();
    void Update();
};

//}

#endif // NULLPHYSICS_H
