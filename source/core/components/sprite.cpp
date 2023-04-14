#include "sprite.h"
#include "core/x-platform/pixmap.h"
#include "core/application.h"

void Sprite::Init(const float x_,
                  const float y_,
                  const float scaleX_,
                  const float scaleY_,
                  const int textureWidth_,
                  const int textureHeight_,
                  const unsigned int quadQuantity)
{
    tag = "Sprite";

    x = x_;
    y = y_;

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
        width = textures[0]->width;
    }

    if (textureHeight_ != 0)
    {
        height = textureHeight_;
    }
    else
    {
        height = textures[0]->height;
    }

    halfWidth  = width  / 2;
    halfHeight = height / 2;

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    Array<String> shaders(2);

    for (unsigned int i = 0; i < quadQuantity; i++)
    {
        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f, -1.0f)));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f, -1.0f)));
        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f,  1.0f)));
        vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f + i * 2.0f,  1.0f)));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f, -1.0f)));
        vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f + i * 2.0f,  1.0f)));
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

    drawable = renderer->CreateDrawable(vertices, indices, shaders, &textures);
    drawable->hasDepth    = false;
    drawable->sendToFront = true;
    drawable->uniformData = uniforms;

    // Send scale uniforms
    Uniform("scaleX", static_cast<float>(scaleX));
    Uniform("scaleY", static_cast<float>(scaleY));

    // Let shaders know the desired section to be used of the sprite sheet
    Uniform("width", static_cast<int>(width));
    Uniform("height", static_cast<int>(height));

    // To get graphics placed correctly, the total sprite sheet size is sent to the shader program
    Uniform("totalWidth", static_cast<int>(textures[0]->width));
    Uniform("totalHeight", static_cast<int>(textures[0]->height));

    // To get graphics placed correctly, the viewport resolution is sent to the shader program
    Uniform("screenWidth", static_cast<int>(renderer->windowWidth));
    Uniform("screenHeight", static_cast<int>(renderer->windowHeight));
}

Sprite::Sprite(String textureFilePath,
               const float _x,
               const float _y,
               const float scaleX_,
               const float scaleY_,
               const int _textureWidth,
               const int _textureHeight,
               const unsigned int quadQuantity)
{
    textures.Add(new Pixmap(textureFilePath));

    Init(_x, _y, scaleX_, scaleY_, _textureWidth, _textureHeight, quadQuantity);
}

Sprite::Sprite(Pixmap *texture,
               const float _x,
               const float _y,
               const int _textureWidth,
               const int _textureHeight)
{
    textures.Add(texture);

    Init(_x, _y, 1.0f, 1.0f, _textureWidth, _textureHeight);
}

Sprite::~Sprite()
{
    renderer->RemoveDrawable(drawable);
}

void Sprite::Update()
{
    Uniform("pos", static_cast<glm::vec2>(glm::vec2(x, y)));
    Uniform("index", static_cast<int>(index));

    if (timer->TimeSinceStarted() > 1000.0f)
    {
        if (width != textures[0]->width || height != textures[0]->height)
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
