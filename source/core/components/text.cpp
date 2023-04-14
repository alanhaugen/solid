#include "text.h"

Text::Text(String text_, float x_, float y_, const float scaleX_, const float scaleY_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_, scaleX_, scaleY_, 5, 11, text_.Length())
{
    text = text_;
    tag = "Text";
}

void Text::Update()
{
    Sprite::Update();

    for (unsigned int i = 0; i < text.Length(); i++)
    {
        Uniform("index" + String(i), static_cast<int>(text[i]));
    }
}

void Text::UpdateAfterPhysics()
{
    Sprite::UpdateAfterPhysics();
}
