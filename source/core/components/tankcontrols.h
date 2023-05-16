#ifndef TANKCONTROLS_H
#define TANKCONTROLS_H

#include "component.h"
#include "actor.h"
#include "mesh.h"

class TankControls : public Component
{
private:
    Actor *actor;
    float speed;
    float turnSpeed;

    Mesh *idle;
    Mesh *walk;
    Mesh *attack;

public:
    TankControls();
    TankControls(Actor *actor);

    void Init();

    void Update();
    void UpdateAfterPhysics();
};

#endif // TANKCONTROLS_H
