#ifndef CUBE_H
#define CUBE_H

#include "core/components/component.h"

class Cube : public Component
{
private:
    IDrawable *drawable;

public:
    Physics::IPhysics::HitBox *collisionBox;
    glm::vec3 pos;

    Cube(float x = 0, float y = 0, float z = 0, float length = 1, float width = 1, float height = 1,
         String texturePath = "",
         bool hasCollision = true,
         glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
         String shaderVertPath = "data/simple.vert",
         String shaderFragPath = "data/simple.frag");
    ~Cube();

    void Hide();
    void Show();

    bool isVisible();

    void Update();
    void UpdateAfterPhysics();
};

#endif // CUBE_H
