#ifndef NULLRENDERER_H
#define NULLRENDERER_H

#include <cstddef>
#include "modules/renderer/renderer.h"

class Pixmap;
class NullRenderer : public IRenderer
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

    virtual void Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray);
    virtual void DrawDebugText(float x, float y, const char* str);

    virtual IDrawable *CreateDrawable(Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            Array<Pixmap *> *textures);

    virtual void RemoveDrawable(IDrawable *drawable);

    virtual void SetDirectionalLight(glm::vec3 direction, ILight *light);
    virtual void ResetDirectionalLight();

    ILight *AddPointLight(glm::vec3 position, glm::vec3 quadricEquation, ILight *light);
    virtual void RemovePointLight(ILight *light);

    virtual void Draw(IDrawable *drawable);

    virtual void Resize(int length, int height);
};

#endif
