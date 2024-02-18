#ifndef GLES2RENDERER_H
#define GLES2RENDERER_H

//namespace Renderer
//{

#include "modules/renderer/null/nullrenderer.h"
#include "modules/renderer/light.h"
#include "core/x-platform/locator.h"
#include "core/x-platform/typedefs.h"
#include "core/containers/array.h"
#include "gles2drawable.h"

class GLES2Renderer : public Renderer::NullRenderer
{
protected:
    void GetError();

private:
    Array<GLES2Drawable*> drawables;
    Array<GLES2Texture*> textures;
    Array<ILight*> lights;

    void renderView(const glm::mat4& projViewMatrix, glm::vec2 viewOffset, glm::vec2 viewSize);

    GLES2Texture *FindTexture(String texturePath);

public:
    virtual bool Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight);
    virtual void Render(const Array<glm::mat4> &projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray);
    virtual void DrawDebugText(float x, float y, const char* str);
    virtual void Resize(int width, int height);

    IDrawable *CreateDrawable(Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            Array<ITexture *> textures);
    IDrawable *CreateDrawable(
            Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            ITexture *texture);
    void RemoveDrawable(IDrawable *drawable);

    ITexture* CreateTexture(String filename);
    ITexture* CreateTexture(String front, String back, String top, String bottom, String left, String right);
    ITexture* CreateTexture(int width, int height);
    void RemoveTexture(ITexture* texture);

    void ClearDrawables();

    ~GLES2Renderer();
};

//}

#endif // GLES2RENDERER_H
