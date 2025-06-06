#include "sprite.h"
#include "core/application.h"

void Sprite::Init(const float x_,
                  const float y_,
                  const float scaleX_,
                  const float scaleY_,
                  const glm::vec2 anchorPoint_,
                  const int textureWidth_,
                  const int textureHeight_,
                  const unsigned int quadQuantity_,
                  const char *glyphs,
                  const float distanceToNextGlyph)
{
    anchorPoint = anchorPoint_;
    quadQuantity = quadQuantity_;

    tag = "Sprite";

    float x = x_;
    float y = y_;

    matrix.Translate(glm::vec3(x, y, 0.0f));

    timer = Application::GetTime("animation");

    index = 0;

    scaleX = scaleX_;
    scaleY = scaleY_;

    originalScaleX = scaleX_;
    originalScaleY = scaleY_;

    isFlipped = false;
    isFlippedVertical = false;

    if (textureWidth_ != 0)
    {
        width = textureWidth_;
    }
    else
    {
        width = texture->width;
    }

    if (textureHeight_ != 0)
    {
        height = textureHeight_;
    }
    else
    {
        height = texture->height;
    }

    halfWidth  = width  / 2;
    halfHeight = height / 2;

    Array<IDrawable::Vertex> vertices(6 * quadQuantity);
    Array<unsigned int> indices;
    Array<String> shaders(2);

    for (unsigned int i = 0; i < quadQuantity; i++)
    {
        int glyph = glyphs[i] - 32; // 32 is to remove control chars, see an ascii table

        if (strcmp(glyphs, "") == 0) // Comparing to 0 means they are equal TODO: Use safe strncmp instead!
        {
            glyph = -1; // -1 will allow the shader to animate the sprite
        }

        vertices[0 + i * 6] = (IDrawable::Vertex(glm::vec2(-1.0f + i * distanceToNextGlyph, -1.0f), glyph));
        vertices[1 + i * 6] = (IDrawable::Vertex(glm::vec2( 1.0f + i * distanceToNextGlyph, -1.0f), glyph));
        vertices[2 + i * 6] = (IDrawable::Vertex(glm::vec2(-1.0f + i * distanceToNextGlyph,  1.0f), glyph));
        vertices[3 + i * 6] = (IDrawable::Vertex(glm::vec2(-1.0f + i * distanceToNextGlyph,  1.0f), glyph));
        vertices[4 + i * 6] = (IDrawable::Vertex(glm::vec2( 1.0f + i * distanceToNextGlyph, -1.0f), glyph));
        vertices[5 + i * 6] = (IDrawable::Vertex(glm::vec2( 1.0f + i * distanceToNextGlyph,  1.0f), glyph));
    }

    for (unsigned int i = 0; i < quadQuantity; i++)
    {
        vertices[0 + i * 6].textureCoordinates = glm::vec2(0, 1);
        vertices[1 + i * 6].textureCoordinates = glm::vec2(1, 1);
        vertices[2 + i * 6].textureCoordinates = glm::vec2(0, 0);
        vertices[3 + i * 6].textureCoordinates = glm::vec2(0, 0);
        vertices[4 + i * 6].textureCoordinates = glm::vec2(1, 1);
        vertices[5 + i * 6].textureCoordinates = glm::vec2(1, 0);

        // NOTE: For some strange reason, the code works (without errors or warnings) without this
        //indices.Add(0 + i * 6);
        //indices.Add(1 + i * 6);
        //indices.Add(2 + i * 6);
        //indices.Add(3 + i * 6);
        //indices.Add(4 + i * 6);
        //indices.Add(5 + i * 6);
    }

    shaders.Insert("data/gui.vert", VERTEX_SHADER);
    shaders.Insert("data/gui.frag", FRAGMENT_SHADER);

    // Create drawable
    drawable = renderer->CreateDrawable(vertices, indices, shaders, texture);
    drawable->hasDepth    = false;
    drawable->sendToFront = true;

    // Send scale uniforms
    drawable->uniforms.scaleX[0] = scaleX;
    drawable->uniforms.scaleY[0] = scaleY;

    // Let shaders know the desired section to be used of the sprite sheet
    drawable->uniforms.width[0] = width;
    drawable->uniforms.height[0] = height;

    // To get graphics placed correctly, the total sprite sheet size is sent to the shader program
    drawable->uniforms.totalWidth[0] = texture->width;
    drawable->uniforms.totalHeight[0] = texture->height;

    // To get graphics placed correctly, the viewport resolution is sent to the shader program
    drawable->uniforms.screenWidth[0] = renderer->windowWidth;
    drawable->uniforms.screenHeight[0] = renderer->windowHeight;

    // Setup if sprite is flipped or not
    drawable->uniforms.flip[0] = isFlipped;
    drawable->uniforms.flipVertical[0] = isFlippedVertical;
}

Sprite::Sprite(String textureFilePath,
               const float _x,
               const float _y,
               const float scaleX_,
               const float scaleY_,
               const glm::vec2 anchorPoint_,
               const int _textureWidth,
               const int _textureHeight,
               const unsigned int quadQuantity_,
               const char *glyphs,
               const float distanceToNextGlyph)
{
    texture = renderer->CreateTexture(textureFilePath);

    Init(_x, _y, scaleX_, scaleY_, anchorPoint_, _textureWidth, _textureHeight, quadQuantity_, glyphs, distanceToNextGlyph);
}

Sprite::Sprite(const int red,
               const int green,
               const int blue,
               const float _x,
               const float _y,
               const float scaleX_,
               const float scaleY_,
               const glm::vec2 anchorPoint_,
               const int _textureWidth,
               const int _textureHeight,
               const unsigned int quadQuantity_,
               const char *glyphs,
               const float distanceToNextGlyph)
{
    //textures.Add(new Pixmap(red, green, blue));

    Init(_x, _y, scaleX_, scaleY_, anchorPoint_, _textureWidth, _textureHeight, quadQuantity_, glyphs, distanceToNextGlyph);
}

Sprite::~Sprite()
{
    if (renderer != NULL) // When the user closes the program, renderer will be deleted before this code will run
    {
        renderer->RemoveDrawable(drawable);
    }
}

void Sprite::Update()
{
    //quadQuantity/2 because each character's given space is half of its width
    transformedX = *matrix.x - (anchorPoint.x * width * scaleX + anchorPoint.x * width * scaleX * (quadQuantity-1)/2);
    transformedY = *matrix.y - anchorPoint.y * height * scaleY + Y_OFFSET;

    //drawable->uniformData = uniforms;
    drawable->uniforms.pos = glm::vec4(transformedX, transformedY, 0, 0);
    drawable->uniforms.index[0] = index;
    drawable->uniforms.flip[0] = isFlipped;
    drawable->uniforms.flipVertical[0] = isFlippedVertical;
    drawable->uniforms.screenWidth[0] = renderer->windowWidth;
    drawable->uniforms.screenHeight[0] = renderer->windowHeight;
    drawable->uniforms.time[0] = Application::time->TimeSinceStarted();
    drawable->uniforms.scaleX[0] = scaleX;
    drawable->uniforms.scaleY[0] = scaleY;
    //Uniform("rotation", static_cast<glm::vec2>(glm::vec2(0.0f, 1.0f)));

    if (timer->TimeSinceStarted() > 100.0f)
    {
        if (width != texture->width || height != texture->height)
        {
            index++;

            if (index > 50)
                index = 0;
        }

        timer->Reset();
    }

    renderer->Draw(drawable);
}

void Sprite::UpdateAfterPhysics()
{
}

void Sprite::AddAnimation(int anim, int frames[], int frames_length, int framerate, bool looping)
{
    /*int col = index % columns;
    int row = rows - 1 - index / rows;
    float s = (float)col / (float)columns;
    float t = (float)row / (float)rows;

    Uniform("s", static_cast<float>(s));
    Uniform("t", static_cast<float>(t));*/
}

bool Sprite::IsPressed()
{
    if (input.Mouse.Pressed)
    {
        if ((input.Mouse.x >= transformedX && input.Mouse.y >= transformedY) &&
                (input.Mouse.x < transformedX + (width * scaleX) && input.Mouse.y < transformedY + (height * scaleY)))
        {
            return true;
        }
    }

    return false;
}

bool Sprite::IsHoveredOver()
{
    if ((input.Mouse.x >= transformedX && input.Mouse.y >= transformedY) &&
            (input.Mouse.x < transformedX + (width * scaleX) && input.Mouse.y < transformedY + (height * scaleY)))
    {
        return true;
    }

    return false;
}

void Sprite::PlayAnimation(int anim, bool reset)
{

}

void Sprite::FlipHorizontal()
{
    isFlipped = !isFlipped;
}

void Sprite::FlipVertical()
{
    isFlippedVertical = !isFlippedVertical;
}

void Sprite::Show()
{
    isVisible = true;
    drawable->visible = true;
}

void Sprite::Hide()
{
    isVisible = false;
    drawable->visible = false;
}

void Sprite::Rotate(int radians)
{

}

void Sprite::Scale(const float scale, bool x, bool y)
{
}
