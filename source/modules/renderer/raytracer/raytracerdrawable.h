#ifndef GLES2DRAWABLE_H
#define GLES2DRAWABLE_H

#include <glad/glad.h>
#include "modules/renderer/drawable.h"
#include "gles2shader.h"
#include "core/x-platform/string.h"
#include "gles2shader.h"
#include <cstddef>

class GLES2Drawable : public IDrawable
{
public:
    GLES2Drawable(GLuint _vaoID,
                  Array<String> &shaders,
                  Array<IFile*> *textures = NULL);
    ~GLES2Drawable();

    void Activate();

    void Upload(void *array, int size);
};

#endif // GLES2DRAWABLE_H
