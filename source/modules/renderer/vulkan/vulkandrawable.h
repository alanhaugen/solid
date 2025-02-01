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

    struct UniformBlock
    {
        glm::vec2 pos;
        float scaleX;
        float scaleY;
        int width;
        int height;
        int totalWidth;
        int totalHeight;
        int index;
        int screenWidth;
        int screenHeight;
        int flip;
        int flipVertical;
        float time;
    };

    AllocatedBuffer uniformBuffer;

    VkDescriptorSet globalDescriptor;

public:
    AllocatedBuffer vertexBuffer;

    struct VertexInputDescription
    {
        std::vector<VkVertexInputBindingDescription> bindings;
        std::vector<VkVertexInputAttributeDescription> attributes;

        VkPipelineVertexInputStateCreateFlags flags = 0;
    };

    VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                   Array<unsigned int> &indices,
                   VulkanShader* shader_,
                   Array<ITexture *> &textures,
                   VmaAllocator allocator);

    int indicesQuantity;
    int verticesQuantity;

    VkPipeline pipeline;

    VertexInputDescription GetVertexDescription();
};

#endif // VULKANDRAWABLE_H
