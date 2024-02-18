#ifndef RENDERER_H
#define RENDERER_H

#include "light.h"
#include "drawable.h"
#include "texture.h"
#include "camera.h"
#include "core/x-platform/string.h"
#include "modules/filesystem/file.h"

namespace Renderer
{

class IRenderer
{
public:
    virtual bool Init(
            bool fullscreen,
            const char *windowTitle,
            const unsigned int windowLength,
            const unsigned int windowHeight) = 0;

    virtual void Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray) = 0;
    virtual void DrawDebugText(float x, float y, const char* str) = 0;
    virtual void Resize(int length, int height) = 0;

    virtual IDrawable *CreateDrawable(
            Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            Array<ITexture*> textures) = 0;

    virtual void RemoveDrawable(IDrawable *drawable) = 0;

    virtual ITexture* CreateTexture(int width, int height) = 0;
    virtual ITexture* CreateTexture(String filename) = 0;
    virtual void RemoveTexture(ITexture* texture) = 0;

    virtual void SetDirectionalLight(glm::vec3 direction, ILight *light) = 0;
    virtual void ResetDirectionalLight() = 0;

    virtual ILight *AddPointLight(glm::vec3 position, glm::vec3 quadricEquation, ILight *light) = 0;
    virtual void RemovePointLight(ILight *light) = 0;

    virtual ICamera* CreateCamera(int viewport,
                                  glm::vec3 position,
                                  glm::vec3 orientation) = 0;
    virtual void RemoveCamera(ICamera* camera) = 0;

    virtual void Draw(IDrawable *drawable) = 0;

    virtual ~IRenderer() {}

    int windowWidth, windowHeight;

    int *framebufferdata;

protected:
    virtual void PreRender() = 0;
    virtual void PostRender() = 0;
};

}

#endif
