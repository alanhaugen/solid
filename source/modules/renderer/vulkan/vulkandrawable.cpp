#include <core/application.h>
#include "vulkandrawable.h"
#include "vulkantexture.h"

//static float TexturesQuantity = 0;

VulkanDrawable::VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                               Array<unsigned int> &indices,
                               VulkanShader* shader_,
                               Array<ITexture *> &textures,
                               VmaAllocator allocator_,
                               VkDevice device_,
                               VkDescriptorPool descriptorPool,
                               VkDescriptorSetLayout& textureSetLayout,
                               AllocatedBuffer uniformBuffer_,
                               int offset_)
{
    // Set the Vulkan Memory Allocator (VMA)
    allocator = allocator_;
    device = device_;
    uniformBuffer = uniformBuffer_;
    offset = offset_;

    if (textures.Size() != 0)
    {
        isTextured = true;

        uniforms.index.y = 1.0f;
        //TexturesQuantity++;

        // Create image sampler (filtering)
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.pNext = nullptr;
        samplerInfo.magFilter = VK_FILTER_NEAREST;
        samplerInfo.minFilter = VK_FILTER_NEAREST;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        vkCreateSampler(device, &samplerInfo, nullptr, &blockySampler);

        // Allocate descriptor set
        VkDescriptorSetAllocateInfo allocInfo ={};
        allocInfo.pNext = nullptr;
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        //using the pool we just set
        allocInfo.descriptorPool = descriptorPool;
        //only 1 descriptor
        allocInfo.descriptorSetCount = 1;
        //using the uniform data layout
        allocInfo.pSetLayouts = &textureSetLayout;

        vkAllocateDescriptorSets(device, &allocInfo, &textureDescriptor);

        VulkanTexture* vTex = static_cast<VulkanTexture*>(textures[0]);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = vTex->imageView;   // created earlier
        imageInfo.sampler   = blockySampler;     // created earlier

        VkWriteDescriptorSet texWrite{};
        texWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        texWrite.dstSet = textureDescriptor;
        texWrite.dstBinding = 0;
        texWrite.descriptorCount = 1;
        texWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        texWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, 1, &texWrite, 0, nullptr);
    }
    else
    {
        isTextured = false;
        uniforms.index.y = -1.0f;
    }

    draw = false;
    lastFrame = 0;

    // Ensure weights for skinning add up to 1 (very important)
    // TODO: Do not do this for meshes without skins
    for (unsigned int i = 0; i < vertices.Size(); i++)
    {
        float weightsSum = vertices[i].weights.x + vertices[i].weights.y + vertices[i].weights.z + vertices[i].weights.w;

        if (weightsSum != 1.0f)
        {
            float missing = 1.0f - weightsSum;
            vertices[i].weights.x += missing / 4;
            vertices[i].weights.y += missing / 4;
            vertices[i].weights.z += missing / 4;
            vertices[i].weights.w += missing / 4;
        }
    }

    indicesQuantity  = indices.Size();
    verticesQuantity = vertices.Size();

    // Allocate vertex buffer
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // This is the total size, in bytes, of the buffer we are allocating
    bufferInfo.size = verticesQuantity * sizeof(Vertex);
    // This buffer is going to be used as a Vertex Buffer
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    // let the VMA library know that this data should be writeable by CPU, but also readable by GPU
    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    // Allocate the buffer
    vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
                    &vertexBuffer.buffer,
                    &vertexBuffer.allocation,
                    nullptr);

    //add the destruction of triangle mesh buffer to the deletion queue
    //_mainDeletionQueue.push_function([=]() {
    //    vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
    //});

    // Upload vertex buffer data
    void* data;
    vmaMapMemory(allocator, vertexBuffer.allocation, &data);
    memcpy(data, &vertices[0], vertices.Size() * sizeof(Vertex));
    vmaUnmapMemory(allocator, vertexBuffer.allocation);

    // Upload index buffer
    if (indicesQuantity != 0)
    {
        vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
                        &indexBuffer.buffer,
                        &indexBuffer.allocation,
                        nullptr);

        void* data;
        vmaMapMemory(allocator, indexBuffer.allocation, &data);
        memcpy(data, &indices[0], indices.Size() * sizeof(unsigned int));
        vmaUnmapMemory(allocator, indexBuffer.allocation);
    }
}

void VulkanDrawable::UploadUniformBufferBlock(const glm::mat4 &projViewMatrix)
{
    //glm::vec3 camPos = { 0.f, 0.f, 10.f };
    //glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
    //projection[1][1] *= -1; // Flip projection because of Vulkan's -Y axis (?)
    //projViewMatrix[1][1] *= -1;

    // Fill a uniform data struct
    uniforms.MVP = projViewMatrix * matrix;
    uniforms.time.x = 20.0f;//fTime += 0.1;

    //and copy it to the buffer
    char* data;
    vmaMapMemory(allocator, uniformBuffer.allocation, (void**)&data);

    data += static_cast<VulkanRenderer*>(Application::renderer)->PadUniformBufferSize(sizeof(UniformBlock)) * offset;

    memcpy(data, &uniforms, sizeof(UniformBlock));

    vmaUnmapMemory(allocator, uniformBuffer.allocation);
}

VulkanDrawable::~VulkanDrawable()
{
    vmaDestroyBuffer(allocator, vertexBuffer.buffer, vertexBuffer.allocation);

    if (indicesQuantity != 0)
    {
        vmaDestroyBuffer(allocator, indexBuffer.buffer, indexBuffer.allocation);
    }

    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyPipeline(device, pipeline, nullptr);
}

VulkanDrawable::VertexInputDescription VulkanDrawable::GetVertexDescription()
{
    VertexInputDescription description;

    // We will have just 1 vertex buffer binding, with a per-vertex rate
    VkVertexInputBindingDescription mainBinding = {};
    mainBinding.binding = 0;
    mainBinding.stride = sizeof(Vertex);
    mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    description.bindings.push_back(mainBinding);

    // Position will be stored at Location 0
    VkVertexInputAttributeDescription positionAttribute = {};
    positionAttribute.binding = 0;
    positionAttribute.location = 0;
    positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    positionAttribute.offset = offsetof(Vertex, position);

    // Color will be stored at Location 1
    VkVertexInputAttributeDescription colorAttribute = {};
    colorAttribute.binding = 0;
    colorAttribute.location = 1;
    colorAttribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    colorAttribute.offset = offsetof(Vertex, color);

    // Normal will be stored at Location 2
    VkVertexInputAttributeDescription normalAttribute = {};
    normalAttribute.binding = 0;
    normalAttribute.location = 2;
    normalAttribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    normalAttribute.offset = offsetof(Vertex, normal);

    // Texcoord will be stored at Location 3
    VkVertexInputAttributeDescription texCoordAttribute = {};
    texCoordAttribute.binding = 0;
    texCoordAttribute.location = 3;
    texCoordAttribute.format = VK_FORMAT_R32G32_SFLOAT;
    texCoordAttribute.offset = offsetof(Vertex, textureCoordinates);

    // Weights (for skinning together with joints below) will be stored at Location 4
    VkVertexInputAttributeDescription weightsAttribute = {};
    weightsAttribute.binding = 0;
    weightsAttribute.location = 4;
    weightsAttribute.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    weightsAttribute.offset = offsetof(Vertex, weights);

    // Joints (for skinning together with weights above) will be stored at Location 5
    VkVertexInputAttributeDescription jointsAttribute = {};
    jointsAttribute.binding = 0;
    jointsAttribute.location = 5;
    jointsAttribute.format = VK_FORMAT_R32G32B32A32_SINT;
    jointsAttribute.offset = offsetof(Vertex, joints);

    // Glyph for text will be stored at Location 6
    VkVertexInputAttributeDescription glyphAttribute = {};
    glyphAttribute.binding = 0;
    glyphAttribute.location = 6;
    glyphAttribute.format = VK_FORMAT_R8_SINT;
    glyphAttribute.offset = offsetof(Vertex, glyph);

    // Glyph for text will be stored at Location 6
    description.attributes.push_back(positionAttribute);
    description.attributes.push_back(colorAttribute);
    description.attributes.push_back(normalAttribute);
    description.attributes.push_back(texCoordAttribute);
    description.attributes.push_back(weightsAttribute);
    description.attributes.push_back(jointsAttribute);
    description.attributes.push_back(glyphAttribute);

    return description;
}
