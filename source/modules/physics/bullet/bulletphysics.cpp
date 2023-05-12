#include "bulletphysics.h"

BulletPhysics::BulletPhysics()
{
    //collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = new btDefaultCollisionConfiguration();

    //use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    //btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = new btDbvtBroadphase();

    //the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    //create a few basic rigid bodies

    //the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

    {
        //create a dynamic rigidbody

        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(2, 10, 0));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }
}

BulletPhysics::~BulletPhysics()
{
    ///-----stepsimulation_end-----

    //cleanup in the reverse order of creation/initialization

    ///-----cleanup_start-----
    //remove the rigidbodies from the dynamics world and delete them
    for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    //delete dynamics world
    delete dynamicsWorld;

    //delete solver
    delete solver;

    //delete broadphase
    delete overlappingPairCache;

    //delete dispatcher
    delete dispatcher;

    delete collisionConfiguration;

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();
}

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

//! There are a host of fast algorthims at https://www.realtimerendering.com/intersections.html
bool BulletPhysics::Intersect(const Ray &ray, const HitBox *hitbox)
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

Physics::IPhysics::Collider *BulletPhysics::Collide(HitBox *hitbox, const char *type)
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

Physics::IPhysics::HitBox *BulletPhysics::CreateHitBox(glm::vec3 dimensions, const char *type = "solid")
{
    HitBox *hitbox = new HitBox;
    hitbox->dimensions = dimensions;
    hitbox->type = type;

    Collider *collider = new Collider;
    collider->hitbox = hitbox;

    colliders.Add(collider);

    return hitbox;
}

void BulletPhysics::Update()
{
    // Do some simulation

    //-----stepsimulation_start-----
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }
        printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
    }
}
