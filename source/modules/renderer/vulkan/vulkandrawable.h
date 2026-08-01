#ifndef VULKANRAWABLE_H
#define VULKANRAWABLE_H

#include "modules/renderer/null/nulldrawable.h"
#include "vulkanshader.h"

#include "vk_mem_alloc.h"
#include <vector>

struct AllocatedBuffer
{
    VkBuffer buffer;
    VmaAllocation allocation;
};

class VulkanDrawable : public NullDrawable
{
private:
    AllocatedBuffer uniformBuffer;

    VkDevice device;
    VmaAllocator allocator;
    VmaAllocation allocation;
    float fTime = 0.0f;

public:
    AllocatedBuffer vertexBuffer;
    AllocatedBuffer indexBuffer;

    void UploadUniformBufferBlock(const glm::mat4& projViewMatrix);

    int offset;

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
                   VmaAllocator allocator,
                   VkDevice device,
                   VkDescriptorPool descriptorPool,
                   VkDescriptorSetLayout& textureSetLayout,
                   AllocatedBuffer uniformBuffer_,
                   int offset_);

    ~VulkanDrawable();

    int indicesQuantity;
    int verticesQuantity;

    VkSampler blockySampler;
    VkDescriptorSet textureDescriptor;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;

    VertexInputDescription GetVertexDescription();
};

#endif // VULKANDRAWABLE_H
