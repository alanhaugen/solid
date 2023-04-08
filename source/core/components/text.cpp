#include "text.h"

Text::Text(String text_, float x_, float y_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_, 1.0f, 1.0f, 32, 32)
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
