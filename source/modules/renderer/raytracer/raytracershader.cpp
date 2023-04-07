#include "gles2shader.h"

GLES2Shader::GLES2Shader(GLuint _vaoID) :
      shaders(2)
{
    vaoID = _vaoID;

    shaders[VERTEX_SHADER]   = 0;
    shaders[FRAGMENT_SHADER] = 0;
}

GLES2Shader::~GLES2Shader()
{
    glDeleteProgram(program);
}

void GLES2Shader::ConnectAttribute(unsigned int location, int id)
{
    /*glBindVertexArray(vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, location);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    //glVertexAttribPointer();*/
}

void GLES2Shader::ConnectElement(unsigned int location, int id)
{
    /*glBindVertexArray(vaoID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, location);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);*/
}

void GLES2Shader::ConnectUniform(unsigned int location, int id)
{

}

void GLES2Shader::Compile()
{
    // Attach shaders to the shader program
    program = glCreateProgram();

    if (shaders[VERTEX_SHADER] != 0)
    {
        glAttachShader(program, shaders[VERTEX_SHADER]);
    }

    if (shaders[FRAGMENT_SHADER] != 0)
    {
        glAttachShader(program, shaders[FRAGMENT_SHADER]);
    }

    // Link and check whether the program links fine
    GLint status;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        //GLint infoLogLength;
        //glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        //GLchar *infoLog = new GLchar[infoLogLength];
        //glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        //LogError("Link log: " + infoLog);
        //delete[] infoLog;

        char logg[MAX_STRING_LENGTH];
        glGetProgramInfoLog(program, MAX_STRING_LENGTH, NULL, logg);
        LogError(logg);
    }

    glDeleteShader(shaders[VERTEX_SHADER]);
    glDeleteShader(shaders[FRAGMENT_SHADER]);
}

bool GLES2Shader::LoadGLSL(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);

    if (shader == 0)
    {
        LogError("Failed to create new shader object");

        return false;
    }

    glShaderSource(shader, 1, &source, NULL);

    // Check whether the shader loads fine
    GLint status;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        // TODO: Optimise String class to generate buffers at runtime
        //GLint infoLogLength;
        //glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        //char *infoLog = new GLchar[infoLogLength];
        //glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
        //delete[] infoLog;

        char logg[MAX_STRING_LENGTH];
        glGetShaderInfoLog(shader, MAX_STRING_LENGTH, NULL, logg);

        LogError(logg);

        return false;
    }

    switch(type)
    {
    case GL_VERTEX_SHADER:
        shaders[VERTEX_SHADER] = shader;
        break;
    case GL_FRAGMENT_SHADER:
        shaders[FRAGMENT_SHADER] = shader;
        break;
    }

    return true;
}
