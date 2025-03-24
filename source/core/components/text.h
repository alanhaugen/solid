#ifndef TEXT_H
#define TEXT_H

#include "sprite.h"

class Text : public Sprite
{
private:
    String text;

public:
    Text(String text_,
         const float x_ = 0.0f,
         const float y_ = 50.0f,
         const float scaleX_ = 1.0f,
         const float scaleY_ = 1.0f,
         const glm::vec2 anchorPoint_ = glm::vec2(0,0),
         String textureAtlasPath = "data/sheet.png",
         const float distanceToNextGlyph = 1.0f);
    virtual ~Text() {};
    void Update();
    void UpdateAfterPhysics();
};

#endif // TEXT_H
