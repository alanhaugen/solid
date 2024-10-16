#include "nullrenderer.h"

#include "core/x-platform/typedefs.h"
#include "nulldrawable.h"

void Renderer::NullRenderer::PreRender()
{
}

void Renderer::NullRenderer::PostRender()
{
}

bool Renderer::NullRenderer::Init(
        bool openFullscreened,
        const char *windowTitle,
        const unsigned int windowLength,
        const unsigned int windowHeight_)
{
    (void)windowTitle;
    windowWidth = windowLength;
    windowHeight = windowHeight_;
    actualWindowWidth = windowLength;
    actualWindowHeight = windowHeight_;
    borderWidth = 0;
    boarderHeight = 0;
    globalScaleWidth = 1.0f;
    globalScaleHeight = 1.0f;
    fullscreen = openFullscreened;
    return true;
}

void Renderer::NullRenderer::Render(const Array<glm::mat4> &projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray)
{
    (void)projViewMatrixArray;
    (void)viewBoundsArray;
}

void Renderer::NullRenderer::DrawDebugText(float x, float y, const char* str)
{
    (void)x;
    (void)y;
    (void)str;
}

void Renderer::NullRenderer::Fullscreen()
{
    fullscreen = true;
}

void Renderer::NullRenderer::Windowed()
{
    fullscreen = false;
}

IDrawable *Renderer::NullRenderer::CreateDrawable(Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        Array<String> &shaders,
        Array<ITexture *> textures)
{
    (void)vertices;
    (void)indices;
    (void)shaders;
    (void)textures;

    NullDrawable *drawable = new NullDrawable();

    return drawable;
}

IDrawable *Renderer::NullRenderer::CreateDrawable(Array<IDrawable::Vertex> &vertices, Array<unsigned int> &indices, Array<String> &shaders, ITexture *texture)
{
    NullDrawable *drawable = new NullDrawable();

    return drawable;
}

void Renderer::NullRenderer::SetDirectionalLight(glm::vec3 direction, ILight *light)
{
    UNUSED(direction);
    UNUSED(light);
}

void Renderer::NullRenderer::ResetDirectionalLight()
{
}

ILight *Renderer::NullRenderer::AddPointLight(glm::vec3 position, glm::vec3 quadricEquation, ILight *light)
{
    UNUSED(position);
    UNUSED(quadricEquation);
    UNUSED(light);
    return NULL;
}

void Renderer::NullRenderer::RemovePointLight(ILight *light)
{
}

ICamera* Renderer::NullRenderer::CreateCamera(int viewport, glm::vec3 position, glm::vec3 orientation)
{
    return NULL;
}

void Renderer::NullRenderer::RemoveCamera(ICamera *camera)
{
}

void Renderer::NullRenderer::Draw(IDrawable *drawable)
{
    if (drawable->visible)
    {
        drawable->draw = true;
    }
}

void Renderer::NullRenderer::RemoveDrawable(IDrawable *drawable)
{
    (void)drawable;
}

ITexture *Renderer::NullRenderer::CreateTexture(int width, int height)
{
//    return new NullTexture(width, height);
}

ITexture *Renderer::NullRenderer::CreateTexture(String filename)
{
//    return new NullTexture(filename);
}

ITexture *Renderer::NullRenderer::CreateTexture(String front, String back, String top, String bottom, String left, String right)
{
//    return new NullDrawable();
}

void Renderer::NullRenderer::RemoveTexture(ITexture *texture)
{
}

void Renderer::NullRenderer::Resize(int length, int height)
{
    (void)length;
    (void)height;
}
