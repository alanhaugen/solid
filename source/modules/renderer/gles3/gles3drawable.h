#ifndef GLES3DRAWABLE_H
#define GLES3DRAWABLE_H

#define _USE_MATH_DEFINES
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "modules/renderer/drawable.h"
#include "gles3shader.h"
#include "core/x-platform/string.h"
#include "gles3shader.h"
#include "gles3texture.h"
#include <cstddef> // offsetof macro
#include "modules/renderer/null/nulldrawable.h"

class GLES3Drawable : public NullDrawable
{
private:
    int GetUniform(String location);

public:
    GLES3Drawable(Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            GLES3Shader* shader_,
            Array<ITexture *> &textures);
    ~GLES3Drawable();

    void Activate(const glm::mat4 &projViewMatrix);
    void DeActivate();

    GLES3Shader* shader;
    Array<GLES3Texture *> textures;

    GLuint vao, vbo, ibo;
    GLsizei indicesQuantity;
    GLsizei verticesQuantity;

    void Uniform(String location, glm::f32   uniform);
    void Uniform(String location, glm::vec2  uniform);
    void Uniform(String location, glm::vec3  uniform);
    void Uniform(String location, glm::vec4  uniform);
    void Uniform(String location, glm::int32 uniform);
    void Uniform(String location, glm::ivec2 uniform);
    void Uniform(String location, glm::ivec3 uniform);
    void Uniform(String location, glm::ivec4 uniform);
    void Uniform(String location, glm::uint  uniform);
    void Uniform(String location, glm::uvec2 uniform);
    void Uniform(String location, glm::uvec3 uniform);
    void Uniform(String location, glm::uvec4 uniform);
    void Uniform(String location, glm::f32   &uniform);
    void Uniform(String location, glm::vec2  &uniform);
    void Uniform(String location, glm::vec3  &uniform);
    void Uniform(String location, glm::vec4  &uniform);
    void Uniform(String location, glm::int32 &uniform);
    void Uniform(String location, glm::ivec2 &uniform);
    void Uniform(String location, glm::ivec3 &uniform);
    void Uniform(String location, glm::ivec4 &uniform);
    void Uniform(String location, glm::uint  &uniform);
    void Uniform(String location, glm::uvec2 &uniform);
    void Uniform(String location, glm::uvec3 &uniform);
    void Uniform(String location, glm::uvec4 &uniform);
    void Uniform(String location, glm::mat2  &uniform);
    void Uniform(String location, glm::mat3  &uniform);
    void Uniform(String location, glm::mat4  &uniform, int arraySize = 1);
    void Uniform(String location, glm::mat2x3 &uniform);
    void Uniform(String location, glm::mat3x2 &uniform);
    void Uniform(String location, glm::mat2x4 &uniform);
    void Uniform(String location, glm::mat4x2 &uniform);
    void Uniform(String location, glm::mat3x4 &uniform);
    void Uniform(String location, glm::mat4x3 &uniform);
};

#endif // GLES3DRAWABLE_H
