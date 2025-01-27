#ifndef VULKANRAWABLE_H
#define VULKANRAWABLE_H

#include "modules/renderer/null/nulldrawable.h"
#include "vulkanshader.h"

class VulkanDrawable : public NullDrawable
{
public:
    VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                   Array<unsigned int> &indices,
                   VulkanShader* shader_,
                   Array<ITexture *> &textures);
};

#endif // VULKANDRAWABLE_H
