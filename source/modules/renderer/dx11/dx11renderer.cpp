#include "dx11renderer.h"

#include "core/x-platform/pixmap.h"
#include "core/x-platform/typedefs.h"
#include "dx11drawable.h"

void DX11Renderer::PreRender()
{
}

void DX11Renderer::PostRender()
{
}

bool DX11Renderer::Init(
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

void DX11Renderer::Render(const Array<glm::mat4> &projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray)
{
    (void)projViewMatrixArray;
    (void)viewBoundsArray;
}

void DX11Renderer::DrawDebugText(float x, float y, const char* str)
{
    (void)x;
    (void)y;
    (void)str;
}

IDrawable *DX11Renderer::CreateDrawable(
        Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        Array<String> &shaders,
        Array<Pixmap*> *textures)
{
    (void)vertices;
    (void)indices;
    (void)shaders;
    (void)textures;

    DX11Drawable *drawable = new DX11Drawable();

    return drawable;
}

void DX11Renderer::SetDirectionalLight(glm::vec3 direction, ILight *light)
{
    UNUSED(direction);
    UNUSED(light);
}

void DX11Renderer::ResetDirectionalLight()
{
}

ILight *DX11Renderer::AddPointLight(glm::vec3 position, glm::vec3 quadricEquation, ILight *light)
{
    UNUSED(position);
    UNUSED(quadricEquation);
    UNUSED(light);
    return NULL;
}

void DX11Renderer::RemovePointLight(ILight *light)
{
}

void DX11Renderer::Draw(IDrawable *drawable)
{
    if (drawable->visible)
    {
        drawable->draw = true;
    }
}

void DX11Renderer::RemoveDrawable(IDrawable *drawable)
{
    (void)drawable;
}

void DX11Renderer::Resize(int length, int height)
{
    (void)length;
    (void)height;
}

