#ifndef VULKANRAWABLE_H
#define VULKANRAWABLE_H

#include "modules/renderer/null/nulldrawable.h"
#include "vulkanshader.h"

#include "vk_mem_alloc.h"
#include <vector>

static float fTime = 0.0f;

struct AllocatedBuffer
{
    VkBuffer buffer;
    VmaAllocation allocation;
};

struct UniformBlock
{
    glm::mat4 MVP;	// combined modelview projection matrix
    glm::vec4 colour;
    glm::vec4 time;
    glm::vec4 index;
    glm::vec4 pos;
    glm::vec4 scaleX;
    glm::vec4 scaleY;
    glm::vec4 width;
    glm::vec4 height;
    glm::vec4 totalWidth;
    glm::vec4 totalHeight;
    glm::vec4 screenWidth;
    glm::vec4 screenHeight;
    glm::vec4 flip;
    glm::vec4 flipVertical;
    glm::vec4 colourTint;
    glm::mat4 modelMat;
    glm::mat4 normalMat;
    glm::vec4 lightPosition;
    glm::vec4 cameraPosition;
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
