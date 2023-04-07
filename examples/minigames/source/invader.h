#ifndef INVADER_H
#define INVADER_H

#include <core/x-platform/actor.h>
#include <core/components/sprite.h>
#include <core/components/component.h>

class Invader : public IComponent
{
public:
    Invader(Actor &invader);
    ~Invader();

    virtual void Update();

private:
    Actor &invader_;

    bool isActive_;
    Sprite *Bullet_;
    Sprite *sprite_;

    float pos_;
    int speed_;

    int dir_;

    int startPosX_;
    int startPosY_;
};

#endif
