#include "component.h"

Component::Component()
{
    uniforms = new Array<Ptr<UniformData *> >();
}

void Component::Uniform(String location, glm::f32 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec2 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec3 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec4 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::int32 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec2 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec3 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec4 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uint uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec2 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec3 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec4 uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::f32 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::vec4 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::int32 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::ivec4 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uint &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::uvec4 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat4 &uniform, int arraySize) // TODO: Fixme
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat2x3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat3x2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat2x4 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat4x2 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat3x4 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}

void Component::Uniform(String location, glm::mat4x3 &uniform)
{
    (*uniforms)->Add(new UniformData(location, uniform));
}
