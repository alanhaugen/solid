#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "component.h"

class Triangle : public Component
{
public:
    Triangle();
    Triangle(IDrawable::Vertex v1, IDrawable::Vertex v2, IDrawable::Vertex v3);

    void Update();
    void UpdateAfterPhysics();

private:
    IDrawable *drawable;
};

#endif // TRIANGLE_H
