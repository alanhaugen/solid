#ifndef NULLDRAWABLE_H
#define NULLDRAWABLE_H

#include "modules/renderer/drawable.h"

class DX11Drawable : public IDrawable
{
public:
    DX11Drawable();

    void Hide();

    void Show();

    void Uniform(String location, glm::f32   uniform);
    void Uniform(String location, glm::vec2  uniform);
    void Uniform(String location, glm::vec4  uniform);
    void Uniform(String location, glm::int32 uniform);
    void Uniform(String location, glm::ivec2 uniform);
    void Uniform(String location, glm::ivec3 uniform);
    void Uniform(String location, glm::ivec4 uniform);
    void Uniform(String location, glm::uint  uniform);
    void Uniform(String location, glm::uvec2 uniform);
    void Uniform(String location, glm::vec3  uniform);
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
    void Uniform(String location, glm::mat4  &uniform);
    void Uniform(String location, glm::mat2x3 &uniform);
    void Uniform(String location, glm::mat3x2 &uniform);
    void Uniform(String location, glm::mat2x4 &uniform);
    void Uniform(String location, glm::mat4x2 &uniform);
    void Uniform(String location, glm::mat3x4 &uniform);
    void Uniform(String location, glm::mat4x3 &uniform);
};

#endif // NULLDRAWABLE_H
