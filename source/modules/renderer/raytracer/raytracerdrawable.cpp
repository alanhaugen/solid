#include "gles2drawable.h"

GLES2Drawable::GLES2Drawable(
        GLuint vaoID,
        Array<String> &shaders,
        Array<IFile *> *textures)
{
    shader = new GLES2Shader(vaoID);

    GLES2Shader *gles2Shader = dynamic_cast<GLES2Shader*>(shader);

    gles2Shader->LoadGLSL(GL_VERTEX_SHADER, shaders[VERTEX_SHADER]);
    gles2Shader->LoadGLSL(GL_FRAGMENT_SHADER, shaders[FRAGMENT_SHADER]);

    gles2Shader->Compile();
}

GLES2Drawable::~GLES2Drawable()
{
    delete shader;
}

// TODO: Check if fails
void GLES2Drawable::Activate()
{
    /*glUseProgram(shader->program);

    glBindVertexArray(shader->vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);*/
}

void GLES2Drawable::Upload(void *array, int size)
{
    // TODO: Upload data to GPU
}
