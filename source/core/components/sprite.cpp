#include "sprite.h"
#include "core/application.h"

void Sprite::Init(const float x_,
                  const float y_,
                  const float scaleX_,
                  const float scaleY_,
                  const int textureWidth_,
                  const int textureHeight_,
                  const unsigned int quadQuantity,
                  const char *glyphs)
{
    tag = "Sprite";

    float x = x_;
    float y = y_;

    matrix.Translate(glm::vec3(x, y, 0.0f));

    timer = Application::GetTime("animation");

    index = 0;

    scaleX = scaleX_;
    scaleY = scaleY_;

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

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    Array<String> shaders(2);

    for (unsigned int i = 0; i < quadQuantity; i++)
    {
        int glyph = glyphs[i] - 32; // 32 is to remove control chars, see an ascii table

        if (strcmp(glyphs, "") == 0) // Comparing to 0 means they are equal TODO: Use safe strncmp instead!
        {
            glyph = -1; // -1 will allow the shader to animate the sprite
        }

        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f, -1.0f), glyph));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f, -1.0f), glyph));
        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f,  1.0f), glyph));
        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f,  1.0f), glyph));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f, -1.0f), glyph));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f,  1.0f), glyph));
    }

    for (unsigned int i = 0; i < quadQuantity; i++)
    {
        vertices[0 + i * 6].textureCoordinates = glm::vec2(0, 1);
        vertices[1 + i * 6].textureCoordinates = glm::vec2(1, 1);
        vertices[2 + i * 6].textureCoordinates = glm::vec2(0, 0);
        vertices[3 + i * 6].textureCoordinates = glm::vec2(0, 0);
        vertices[4 + i * 6].textureCoordinates = glm::vec2(1, 1);
        vertices[5 + i * 6].textureCoordinates = glm::vec2(1, 0);

        indices.Add(0 + i * 6);
        indices.Add(1 + i * 6);
        indices.Add(2 + i * 6);
        indices.Add(3 + i * 6);
        indices.Add(4 + i * 6);
        indices.Add(5 + i * 6);
    }

    IFile *simpleVertShader = filesystem->Open(URL("data/gui.vert"), PLAIN_TEXT);
    IFile *simpleFragShader = filesystem->Open(URL("data/gui.frag"), PLAIN_TEXT);

    shaders.Insert(simpleVertShader->Read(), VERTEX_SHADER);
    shaders.Insert(simpleFragShader->Read(), FRAGMENT_SHADER);

    delete simpleVertShader;
    delete simpleFragShader;

    // Send scale uniforms
    Uniform("scaleX", static_cast<float>(scaleX));
    Uniform("scaleY", static_cast<float>(scaleY));

    // Let shaders know the desired section to be used of the sprite sheet
    Uniform("width", static_cast<int>(width));
    Uniform("height", static_cast<int>(height));

    // To get graphics placed correctly, the total sprite sheet size is sent to the shader program
    Uniform("totalWidth", static_cast<int>(texture->width));
    Uniform("totalHeight", static_cast<int>(texture->height));

    // To get graphics placed correctly, the viewport resolution is sent to the shader program
    Uniform("screenWidth", static_cast<int>(renderer->windowWidth));
    Uniform("screenHeight", static_cast<int>(renderer->windowHeight));

    // Create drawable
    drawable = renderer->CreateDrawable(vertices, indices, shaders, texture);
    drawable->hasDepth    = false;
    drawable->sendToFront = true;
    drawable->uniformData = uniforms;
}

Sprite::Sprite(String textureFilePath,
               const float _x,
               const float _y,
               const float scaleX_,
               const float scaleY_,
               const int _textureWidth,
               const int _textureHeight,
               const unsigned int quadQuantity,
               const char *glyphs)
{
    texture = renderer->CreateTexture(textureFilePath);

    Init(_x, _y, scaleX_, scaleY_, _textureWidth, _textureHeight, quadQuantity, glyphs);
}

/*Sprite::Sprite(Pixmap *texture,
               const float _x,
               const float _y,
               const int _textureWidth,
               const int _textureHeight)
{
    textures.Add(texture);

    Init(_x, _y, 1.0f, 1.0f, _textureWidth, _textureHeight);
}*/

Sprite::Sprite(const int red,
               const int green,
               const int blue,
               const float _x,
               const float _y,
               const float scaleX_,
               const float scaleY_,
               const int _textureWidth,
               const int _textureHeight,
               const unsigned int quadQuantity,
               const char *glyphs)
{
    //textures.Add(new Pixmap(red, green, blue));

    Init(_x, _y, scaleX_, scaleY_, _textureWidth, _textureHeight, quadQuantity, glyphs);
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
    Uniform("pos", static_cast<glm::vec2>(glm::vec2(*matrix.x, *matrix.y)));
    Uniform("index", static_cast<int>(index));

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

void Sprite::PlayAnimation(int anim, bool reset)
{

}

void Sprite::FlipHorizontal()
{

}

void Sprite::FlipVertical()
{

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
