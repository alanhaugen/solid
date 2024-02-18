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
        bool openWindowed,
        const char *windowTitle,
        const unsigned int windowLength,
        const unsigned int windowHeight)
{
    (void)openWindowed;
    (void)windowTitle;
    (void)windowLength;
    (void)windowHeight;
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

}

ITexture *Renderer::NullRenderer::CreateTexture(String filename)
{

}

void Renderer::NullRenderer::RemoveTexture(ITexture *texture)
{

}

void Renderer::NullRenderer::Resize(int length, int height)
{
    (void)length;
    (void)height;
}
