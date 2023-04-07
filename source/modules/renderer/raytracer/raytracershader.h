#ifndef GLES2SHADER_H
#define GLES2SHADER_H

#include <glad/glad.h>
#include "core/x-platform/locator.h"
#include "core/containers/array.h"
#include "core/containers/hash.h"
#include "modules/renderer/drawable.h"

class GLES2Shader : public IDrawable::Shader
{
public:
    GLES2Shader(GLuint _vaoID);
    ~GLES2Shader();

    bool LoadGLSL(GLenum type, const char *source);

    void Compile();

    void ConnectAttribute(unsigned int location, int id);
    void ConnectElement(unsigned int location, int id);
    void ConnectUniform(unsigned int location, int id);

private:
    Array<GLuint> shaders;
    GLuint vaoID;
};

#endif // GLES2SHADER_H
