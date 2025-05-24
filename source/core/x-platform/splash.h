#ifndef SPLASH_H
#define SPLASH_H

#include "core/x-platform/scene.h"
#include "core/components/sprite.h"
#include "core/components/text.h"

class Splash : public IScene
{
private:
    ITime *timer;

public:
    Splash();
    ~Splash();

    void Init();
    void Update(float dt);
    void UpdateAfterPhysics();
};

#endif // SPLASH_H
