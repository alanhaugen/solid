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
        glm::mat4 MVP;	// combined modelview projection matrix
        glm::vec4 colour;
    };

    AllocatedBuffer CreateBuffer(size_t allocSize,
                                 VkBufferUsageFlags usage,
                                 VmaMemoryUsage memoryUsage);

    AllocatedBuffer uniformBuffer;

    VkDevice device;
    VmaAllocator allocator;

public:
    AllocatedBuffer vertexBuffer;
    VkDescriptorSet descriptor;
    VkDescriptorSetLayout setLayout;

    void UploadUniformBufferBlock();

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
                   VkDescriptorPool descriptorPool);

    ~VulkanDrawable();

    int indicesQuantity;
    int verticesQuantity;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;

    VertexInputDescription GetVertexDescription();
};

#endif // VULKANDRAWABLE_H
