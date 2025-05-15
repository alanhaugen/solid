#ifndef GLES2SHADER_H
#define GLES2SHADER_H

#include <glad/gl.h>
#include "core/x-platform/locator.h"
#include "core/containers/array.h"
#include "core/containers/hash.h"
#include "modules/renderer/drawable.h"

class GLES2Shader
{
public:
    GLES2Shader();
    ~GLES2Shader();

    unsigned int program;
    bool isCompiled;
    String name;

    bool LoadGLSL(GLenum type, const char *source);

    void Compile();

private:
    Array<GLuint> shaders;
};

#endif // GLES2SHADER_H
