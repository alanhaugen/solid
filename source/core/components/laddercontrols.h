#ifndef LADDERCONTROLS_H
#define LADDERCONTROLS_H

#include "component.h"
#include "actor.h"
#include "mesh.h"

class LadderControls : public Component
{
private:
    Actor *actor;
    float speed;
    float turnSpeed;

    bool isFacingLeft;
    bool isJumping;

    float groundY;

    float momentumX;
    float momentumY;

    Mesh *idle;
    Mesh *walk;
    Mesh *attack;

public:
    LadderControls();
    LadderControls(Actor *actor);

    void Init();

    void Update();
    void UpdateAfterPhysics();
};

#endif // LADDERCONTROLS_H
