#ifndef SUPERBIBLE_H
#define SUPERBIBLE_H

#include <core/x-platform/scene.h>
#include <core/x-platform/actor.h>

class Example1 : public IScene
{
private:
    IDrawable *drawable;

public:
    Example1();

    void Update();
    void UpdateLate();
};

#endif // SUPERBIBLE_H
