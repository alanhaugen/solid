#ifndef TEXT_H
#define TEXT_H

#include "sprite.h"

class Text : public Sprite
{
private:
    IDrawable *drawable;
    String text;

public:
    Text(String text_, const float x_ = 0.0f, const float y_ = 0.0f, String textureAtlasPath = "data/gnsh-bitmapfont.png");//"data/cursor.png");//"data/8bit_font.png");
    void Update();
    void UpdateAfterPhysics();
};

#endif // TEXT_H
