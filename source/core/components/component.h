#ifndef COMPONENT_H
#define COMPONENT_H

#include "core/x-platform/locator.h"
#include "core/x-platform/uniform.h"
#include "core/containers/array.h"
#include "core/x-platform/mat.h"
#include "core/x-platform/ptr.h"

class Component : public Locator
{
public:
    Component();
    ~Component();

    Ptr<Array<Ptr<UniformData *> > *> uniforms;

    Mat matrix;
    String tag;

    virtual void Update() = 0;
    virtual void UpdateAfterPhysics() = 0;

    UniformData* GetUniform(String location);

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
    void UniformP(String location, glm::f32   &uniform);
    void UniformP(String location, glm::vec2  &uniform);
    void UniformP(String location, glm::vec3  &uniform);
    void UniformP(String location, glm::vec4  &uniform);
    void UniformP(String location, glm::int32 &uniform);
    void UniformP(String location, glm::ivec2 &uniform);
    void UniformP(String location, glm::ivec3 &uniform);
    void UniformP(String location, glm::ivec4 &uniform);
    void UniformP(String location, glm::uint  &uniform);
    void UniformP(String location, glm::uvec2 &uniform);
    void UniformP(String location, glm::uvec3 &uniform);
    void UniformP(String location, glm::uvec4 &uniform);
    void UniformP(String location, glm::mat2  &uniform);
    void UniformP(String location, glm::mat3  &uniform);
    void UniformP(String location, glm::mat4  &uniform, int arraySize = 1);
    void UniformP(String location, glm::mat2x3 &uniform);
    void UniformP(String location, glm::mat3x2 &uniform);
    void UniformP(String location, glm::mat2x4 &uniform);
    void UniformP(String location, glm::mat4x2 &uniform);
    void UniformP(String location, glm::mat3x4 &uniform);
    void UniformP(String location, glm::mat4x3 &uniform);

};

#endif // COMPONENT_H
