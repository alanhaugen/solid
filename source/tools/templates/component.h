#ifndef Component_H
#define Component_H

#include <core/x-platform/component.h>

class Component : public Component
{
public:
    Component();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

#endif
