#ifndef NULLRENDERER_H
#define NULLRENDERER_H

#include <cstddef>
#include "modules/renderer/renderer.h"

class Pixmap;
class DX11Renderer : public Renderer::IRenderer
{
protected:
    void PreRender();

    void PostRender();

public:
    virtual bool Init(
            bool openWindowed,
            const char *windowTitle,
            const unsigned int windowLength,
            const unsigned int windowHeight);
};

#endif
