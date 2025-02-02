#include "vulkandrawable.h"

VulkanDrawable::AllocatedBuffer VulkanDrawable::CreateBuffer(size_t allocSize,
                                                             VkBufferUsageFlags usage,
                                                             VmaMemoryUsage memoryUsage)
{
    // Allocate buffer
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;

    bufferInfo.size = allocSize;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = memoryUsage;

    AllocatedBuffer newBuffer;

    // Allocate the buffer
    vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
        &newBuffer.buffer,
        &newBuffer.allocation,
        nullptr);

    return newBuffer;
}

VulkanDrawable::VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                               Array<unsigned int> &indices,
                               VulkanShader* shader_,
                               Array<ITexture *> &textures,
                               VmaAllocator allocator_,
                               VkDevice device_,
                               VkDescriptorPool descriptorPool)
{
    // Set the Vulkan Memory Allocator (VMA)
    allocator = allocator_;
    device = device_;

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

    // Allocate buffer block data
    uniformBuffer = CreateBuffer(sizeof(UniformBlock),
                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                 VMA_MEMORY_USAGE_CPU_TO_GPU);

    // Information about the binding.
    VkDescriptorSetLayoutBinding bufferBinding = {};
    bufferBinding.binding = 0;
    bufferBinding.descriptorCount = 1;
    // it's a uniform buffer binding
    bufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    // we use it from the vertex shader
    bufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo setinfo = {};
    setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setinfo.pNext = nullptr;

    //we are going to have 1 binding
    setinfo.bindingCount = 1;
    //no flags
    setinfo.flags = 0;
    //point to the camera buffer binding
    setinfo.pBindings = &bufferBinding;

    vkCreateDescriptorSetLayout(device, &setinfo, nullptr, &setLayout);

    // Allocate descriptor set
    VkDescriptorSetAllocateInfo allocInfo ={};
    allocInfo.pNext = nullptr;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    //using the pool we just set
    allocInfo.descriptorPool = descriptorPool;
    //only 1 descriptor
    allocInfo.descriptorSetCount = 1;
    //using the global data layout
    allocInfo.pSetLayouts = &setLayout;

    vkAllocateDescriptorSets(device, &allocInfo, &descriptor);

    // Point descriptor set to uniform buffer
    VkDescriptorBufferInfo binfo;
    //it will be the uniform buffer
    binfo.buffer = uniformBuffer.buffer;
    //at 0 offset
    binfo.offset = 0;
    //of the size of a camera data struct
    binfo.range = sizeof(UniformBlock);

    VkWriteDescriptorSet setWrite = {};
    setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    setWrite.pNext = nullptr;

    //we are going to write into binding number 0
    setWrite.dstBinding = 0;
    //of the global descriptor
    setWrite.dstSet = descriptor;

    setWrite.descriptorCount = 1;
    //and the type is uniform buffer
    setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    setWrite.pBufferInfo = &binfo;

    vkUpdateDescriptorSets(device, 1, &setWrite, 0, nullptr);

}

void VulkanDrawable::UploadUniformBufferBlock()
{
    // Fill a GPU camera data struct
    UniformBlock uniformData;
    uniformData.MVP = glm::mat4(1.0f);
    uniformData.colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

    //and copy it to the buffer
    void* data;
    vmaMapMemory(allocator, uniformBuffer.allocation, &data);

    memcpy(data, &uniformData, sizeof(UniformBlock));

    vmaUnmapMemory(allocator, uniformBuffer.allocation);
}

VulkanDrawable::~VulkanDrawable()
{
    vkDestroyDescriptorSetLayout(device, setLayout, nullptr);
    vmaDestroyBuffer(allocator, uniformBuffer.buffer, uniformBuffer.allocation);
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
