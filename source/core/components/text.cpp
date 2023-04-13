#include "text.h"

Text::Text(String text_, float x_, float y_, const float scaleX_, const float scaleY_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_, scaleX_, scaleY_, 32, 32)
{
    text = text_;
}

void Text::Update()
{
    Sprite::Update();
}

void Text::UpdateAfterPhysics()
{
    Sprite::UpdateAfterPhysics();
}
