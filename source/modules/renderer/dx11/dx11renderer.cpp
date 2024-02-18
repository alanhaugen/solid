#include "dx11renderer.h"

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
