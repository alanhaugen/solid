#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "core/x-platform/scene.h"
#include "core/x-platform/actor.h"
#include "core/components/mesh.h"

class Primitives : public IScene
{
private:
    Actor cube;
    Actor triangle;
    Actor car;
    Actor teapot;
    Actor suzanne;
    Actor dragon;
    Actor bunny;
    Actor box;

public:
    Primitives();
    void Update(Input *input);
    void UpdateLate(Input *input);
};

#endif // PRIMITIVES_H
