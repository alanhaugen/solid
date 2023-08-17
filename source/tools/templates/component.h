#ifndef Component_H
#define Component_H

#include <core/components/component.h>

class Component : public Component
{
public:
    Component();

    void Update();
    void UpdateAfterPhysics();
};

#endif
