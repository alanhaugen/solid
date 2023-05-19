#ifndef SPLASH_H
#define SPLASH_H

#include "scene.h"
#include "core/components/sprite.h"

class Splash : public IScene
{
private:
    ITime *timer;

public:
    Splash();
    ~Splash();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

#endif // SPLASH_H
