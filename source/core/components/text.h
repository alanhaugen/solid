#ifndef TEXT_H
#define TEXT_H

#include "sprite.h"

class Text : public Sprite
{
private:
    IDrawable *drawable;
    String text;

public:
    Text(String text_,
         const float x_ = 0.0f,
         const float y_ = 50.0f,
         const float scaleX_ = 1.0f,
         const float scaleY_ = 1.0f,
         String textureAtlasPath = "data/sheet.png");
    void Update();
    void UpdateAfterPhysics();
};

#endif // TEXT_H
