#ifndef SCENE_H
#define SCENE_H

#include "locator.h"
#include "core/components/component.h"

class IScene : public Locator
{
public:
    unsigned int id;
    Array<IComponent*> components;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void UpdateAfterPhysics() = 0;

    virtual ~IScene()
    {
        //components.Clear();
    }
};

#endif // SCENE_H
