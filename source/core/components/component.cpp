#include "component.h"

IComponent::IComponent()
{
    uniforms = NULL;
    SetUniforms();
}

IComponent::~IComponent()
{
    delete uniforms;
    uniforms = NULL;
}

void IComponent::SetUniforms(Array<Ptr<UniformData *> > *uniforms_)
{
    delete uniforms;

    if (uniforms_ == NULL)
    {
        uniforms = new Array<Ptr<UniformData *> >();
    }
    else
    {
        uniforms = uniforms_;
    }
}

void IComponent::Uniform(String location, glm::f32 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec2 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec3 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec4 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::int32 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec2 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec3 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec4 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uint uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec2 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec3 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec4 uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::f32 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::vec4 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::int32 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::ivec4 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uint &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::uvec4 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat4 &uniform, int arraySize) // TODO: Fixme
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat2x3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat3x2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat2x4 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat4x2 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat3x4 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}

void IComponent::Uniform(String location, glm::mat4x3 &uniform)
{
    UniformData *object = new UniformData(location, uniform);
    uniforms->Add(Ptr<UniformData *>(&object));
}
