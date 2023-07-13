#ifndef SCENE_H
#define SCENE_H

#include <core/x-platform/scene.h>

class Scene : public IScene
{
public:
    Scene();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

#endif
