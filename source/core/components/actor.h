#ifndef ACTOR_H
#define ACTOR_H

#include "core/x-platform/string.h"
#include "core/components/component.h"
#include "core/containers/linkedlist.h"
#include "core/containers/hash.h"
#include "modules/physics/physics.h"

class Actor : public Component
{
public:
    Actor();
    ~Actor();

    Physics::IPhysics::HitBox *collisionBox;

    void Add(Component *component);

    Component *FindComponent(const char *tag);

    void Update();
//    void UpdateAfterPhysics();

    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;

private:

public:
    Array<Ptr<Component *> > components;
    //Array<UniformData*> uniforms;
    //Hash<String, UniformData*> uniforms;

    //LinkedList<IComponent*> components;
    //Hash<String, IComponent*> componentsMap;
};

#endif // ACTOR_H
