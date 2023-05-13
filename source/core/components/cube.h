#ifndef CUBE_H
#define CUBE_H

#include "core/components/component.h"

class Cube : public IComponent
{
private:
    IDrawable *drawable;

public:
    Cube(float x = 0, float y = 0, float z = 0, float length = 1, float width = 1, float height = 1);
    ~Cube();

    void Update();
    void UpdateAfterPhysics();
};

#endif // CUBE_H
