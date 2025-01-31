#ifndef VULKANRAWABLE_H
#define VULKANRAWABLE_H

#include "modules/renderer/null/nulldrawable.h"
#include "vulkanshader.h"

#include "vk_mem_alloc.h"

class VulkanDrawable : public NullDrawable
{
private:
    struct AllocatedBuffer
    {
        VkBuffer buffer;
        VmaAllocation allocation;
    };

    int indicesQuantity;
    int verticesQuantity;

    AllocatedBuffer vertexBuffer;

public:
    VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                   Array<unsigned int> &indices,
                   VulkanShader* shader_,
                   Array<ITexture *> &textures,
                   VmaAllocator allocator);
};

#endif // VULKANDRAWABLE_H
