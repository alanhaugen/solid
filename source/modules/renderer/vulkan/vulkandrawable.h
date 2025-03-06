#ifndef VULKANRAWABLE_H
#define VULKANRAWABLE_H

#include "modules/renderer/null/nulldrawable.h"
#include "vulkanshader.h"

#include "vk_mem_alloc.h"

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

class VulkanDrawable : public NullDrawable
{
private:
    AllocatedBuffer uniformBuffer;

    VkDevice device;
    VmaAllocator allocator;
    VmaAllocation allocation;

public:
    AllocatedBuffer vertexBuffer;

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
                   VkDescriptorSetLayout setLayout,
                   AllocatedBuffer uniformBuffer_,
                   int offset_);

    ~VulkanDrawable();

    int indicesQuantity;
    int verticesQuantity;

    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;

    VertexInputDescription GetVertexDescription();
};

#endif // VULKANDRAWABLE_H
