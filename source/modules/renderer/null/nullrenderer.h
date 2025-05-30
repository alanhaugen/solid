#ifndef NULLRENDERER_H
#define NULLRENDERER_H

#include <cstddef>
#include "modules/renderer/renderer.h"

namespace Renderer
{

class NullRenderer : public IRenderer
{
protected:
    void PreRender();

    void PostRender();

public:
    virtual bool Init(
            bool openFullscreened,
            const char *windowTitle,
            const unsigned int windowLength,
            const unsigned int windowHeight);

    virtual void Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray);
    virtual void DrawDebugText(float x, float y, const char* str);

    virtual void Fullscreen();
    virtual void Windowed();

    virtual IDrawable *CreateDrawable(Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            Array<ITexture *> textures,
            int topology);
    virtual IDrawable *CreateDrawable(
            Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            ITexture* texture,
            int topology);
    virtual void RemoveDrawable(IDrawable *drawable);

    virtual ITexture* CreateTexture(String filename);
    virtual ITexture* CreateTexture(String front, String back, String top, String bottom, String left, String right);
    virtual ITexture* CreateTexture(int width, int height);
    virtual void RemoveTexture(ITexture* texture);

    virtual void SetDirectionalLight(glm::vec3 direction, ILight *light);
    virtual void ResetDirectionalLight();

    ILight *AddPointLight(glm::vec3 position, glm::vec3 quadricEquation, ILight *light);
    virtual void RemovePointLight(ILight *light);

    virtual ICamera* CreateCamera(int viewport,
                                  glm::vec3 position,
                                  glm::vec3 orientation);
    virtual void RemoveCamera(ICamera* camera);

    virtual void Draw(IDrawable *drawable);

    virtual void Resize(int length, int height);
};

}

#endif
