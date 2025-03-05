#include "vulkandrawable.h"

VulkanDrawable::VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                               Array<unsigned int> &indices,
                               VulkanShader* shader_,
                               Array<ITexture *> &textures,
                               VmaAllocator allocator_,
                               VkDevice device_,
                               VkDescriptorPool descriptorPool,
                               VkDescriptorSetLayout setLayout,
                               AllocatedBuffer uniformBuffer_)
{
    // Set the Vulkan Memory Allocator (VMA)
    allocator = allocator_;
    device = device_;
    uniformBuffer = uniformBuffer_;

    /*for (unsigned int i = 0; i < textures_.Size(); i++)
    {
        GLES2Texture *gles2texture = dynamic_cast<GLES2Texture *>(textures_[i]);

        textures.Add(gles2texture);
    }*/

    if (textures.Size() != 0)
    {
        isTextured = true;
    }
    else
    {
        isTextured = false;
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
}

void VulkanDrawable::UploadUniformBufferBlock(const glm::mat4 &projViewMatrix)
{
    //glm::vec3 camPos = { 0.f, 0.f, 10.f };
    //glm::mat4 projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
    //projection[1][1] *= -1; // Flip projection because of Vulkan's -Y axis (?)

    // Fill a GPU camera data struct
    UniformBlock uniformData;
    uniformData.MVP = projViewMatrix * matrix;
    uniformData.colour = colorTint;

    //and copy it to the buffer
    void* data;
    vmaMapMemory(allocator, uniformBuffer.allocation, &data);

    memcpy(data, &uniformData, sizeof(UniformBlock));

    vmaUnmapMemory(allocator, uniformBuffer.allocation);
}

VulkanDrawable::~VulkanDrawable()
{
    vmaDestroyBuffer(allocator, vertexBuffer.buffer, vertexBuffer.allocation);
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

    description.attributes.push_back(positionAttribute);
    description.attributes.push_back(colorAttribute);
    description.attributes.push_back(normalAttribute);
    description.attributes.push_back(texCoordAttribute);
    description.attributes.push_back(weightsAttribute);
    description.attributes.push_back(jointsAttribute);
    description.attributes.push_back(glyphAttribute);

    return description;
}
