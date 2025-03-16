#include "text.h"

Text::Text(String text_, float x_, float y_, const float scaleX_, const float scaleY_, const glm::vec2 anchorPoint_, String textureAtlasPath)
    : Sprite(textureAtlasPath, x_, y_, scaleX_, scaleY_, anchorPoint_, 64, 64, text_.Length(), text_)
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
