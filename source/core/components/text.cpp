#include "text.h"

Text::Text(String text_, float x_, float y_, const float scaleX_, const float scaleY_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_, scaleX_, scaleY_, 32, 32, text_.Length(), text_)
{
    text = text_;
    tag = "Text";
}

void Text::Update()
{
    Sprite::Update();
}

void Text::UpdateAfterPhysics()
{
    Sprite::UpdateAfterPhysics();
}
