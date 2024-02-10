#include "component.h"

Component::Component()
{
    uniforms = new Array<Ptr<UniformData *> >();
}

// TODO: Convert this to the stack!!! I think that will be a lot faster! Maybe use templates???
UniformData* Component::GetUniform(String location)
{
    for (unsigned int i = 0; i < (*uniforms)->Size(); i++)
    {
        if (uniforms.data->object->array[i].data->object->handle == location)
        {
            return uniforms.data->object->array[i].data->object;
        }

    }

    return NULL;
}

void Component::Uniform(String location, glm::f32 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::f32(uniform));
    }
}

void Component::Uniform(String location, glm::vec2 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec2(uniform));
    }
}

void Component::Uniform(String location, glm::vec3 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec3(uniform));
    }
}

void Component::Uniform(String location, glm::vec4 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec4(uniform));
    }
}

void Component::Uniform(String location, glm::int32 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::int32(uniform));
    }
}

void Component::Uniform(String location, glm::ivec2 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec2(uniform));
    }
}

void Component::Uniform(String location, glm::ivec3 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec3(uniform));
    }
}

void Component::Uniform(String location, glm::ivec4 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec4(uniform));
    }
}

void Component::Uniform(String location, glm::uint uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uint(uniform));
    }
}

void Component::Uniform(String location, glm::uvec2 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec2(uniform));
    }
}

void Component::Uniform(String location, glm::uvec3 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec3(uniform));
    }
}

void Component::Uniform(String location, glm::uvec4 uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec4(uniform));
    }
}

void Component::UniformP(String location, glm::f32 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::f32(uniform));
    }
}

void Component::UniformP(String location, glm::vec2 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec2(uniform));
    }
}

void Component::UniformP(String location, glm::vec3 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec3(uniform));
    }
}

void Component::UniformP(String location, glm::vec4 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::vec4(uniform));
    }
}

void Component::UniformP(String location, glm::int32 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::int32(uniform));
    }
}

void Component::UniformP(String location, glm::ivec2 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec2(uniform));
    }
}

void Component::UniformP(String location, glm::ivec3 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec3(uniform));
    }
}

void Component::UniformP(String location, glm::ivec4 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::ivec4(uniform));
    }
}

void Component::UniformP(String location, glm::uint &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uint(uniform));
    }
}

void Component::UniformP(String location, glm::uvec2 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec2(uniform));
    }
}

void Component::UniformP(String location, glm::uvec3 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec3(uniform));
    }
}

void Component::UniformP(String location, glm::uvec4 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::uvec4(uniform));
    }
}

void Component::UniformP(String location, glm::mat2 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::mat2(uniform));
    }
}

void Component::UniformP(String location, glm::mat3 &uniform)
{
    UniformData* data = GetUniform(location);

    if (data == NULL)
    {
        (*uniforms)->Add(new UniformData(location, uniform));
    }
    else
    {
        data->SetData(new glm::mat3(uniform));
    }
}

void Component::UniformP(String location, glm::mat4 &uniform, int arraySize) // TODO: Fixme
{
}

void Component::UniformP(String location, glm::mat2x3 &uniform)
{
}

void Component::UniformP(String location, glm::mat3x2 &uniform)
{
}

void Component::UniformP(String location, glm::mat2x4 &uniform)
{
}

void Component::UniformP(String location, glm::mat4x2 &uniform)
{
}

void Component::UniformP(String location, glm::mat3x4 &uniform)
{
}

void Component::UniformP(String location, glm::mat4x3 &uniform)
{
}
