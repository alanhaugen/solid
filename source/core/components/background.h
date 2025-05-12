#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "core/components/component.h"
#include "core/components/camera.h"

class Background : public Component
{
public:
    Background();
    ~Background();

    Background(Camera *camera);
    Background(glm::vec3 colour, Camera *camera);

    Background(String texture, Camera *camera);
    Background(String texture, float scrollX_, float scrollY_, Camera *camera);

    Background(String front,
               String back,
               String top,
               String bottom,
               String left,
               String right,
               Camera *camera);

    void Update();
    void UpdateAfterPhysics();

private:
    IDrawable *drawable;
    bool scrolling;

    float scrollX;
    float scrollY;

    float scrollSpeed;

    float f = 0.0f;

    Camera *activeCamera;

    ITexture *texture;

    void Init();
};

#endif // BACKGROUND_H
