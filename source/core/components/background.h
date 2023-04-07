#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "core/components/actor.h"
#include "core/components/camera.h"
#include "core/x-platform/pixmap.h"

class Background : public Actor
{
public:
    Background();
    ~Background();

    Background(Camera *camera);
    Background(glm::vec3 colour, Camera *camera);

    Background(String texture, Camera *camera);
    Background(Pixmap texture, float scrollX_, float scrollY_, Camera *camera);

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
    Array<Pixmap*> *textures;
    bool scrolling;

    float scrollX;
    float scrollY;

    Camera *activeCamera;

    void Init();
};

#endif // BACKGROUND_H
