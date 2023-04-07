#include "text.h"

Text::Text(String text_, float x_, float y_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_)
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
