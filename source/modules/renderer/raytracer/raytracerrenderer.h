#ifndef GLES2RENDERER_H
#define GLES2RENDERER_H

#include <glad/glad.h>
#include "modules/renderer/renderer.h"
#include "core/x-platform/locator.h"
#include "core/containers/array.h"
#include "gles2drawable.h"
#include "glm/mat4x4.hpp"

const unsigned int MAX_VAOS = 256;
const unsigned int MAX_BUFFERS = 256;

class GLES2Renderer : public IRenderer
{
protected:
    GLuint vaos[MAX_VAOS];
    GLuint buffers[MAX_BUFFERS];

    void GetError();

private:
    void Draw(IDrawable *drawable);

    void Setup(GLES2Drawable *drawable);

    // Projection and modelview matrices
    glm::mat4 P;
    glm::mat4 MV;

    Array<GLES2Drawable*> drawables;

public:
    bool Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight);

    void Resize(int width, int height);

    void Render();

    IDrawable *CreateDrawable(Array<IDrawable::Vertex> &vertices, Array<unsigned int> &indices, Array<String> &shaders, Array<IFile *> *textures = NULL);

    void RemoveDrawable(IDrawable *drawable);

    void ClearDrawables();
};

#endif // GLES2RENDERER_H
