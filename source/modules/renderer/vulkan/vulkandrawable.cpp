#include "vulkandrawable.h"

VulkanDrawable::VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                               Array<unsigned int> &indices,
                               VulkanShader* shader_,
                               Array<ITexture *> &textures,
                               VmaAllocator allocator)
{
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

    //allocate vertex buffer
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    //this is the total size, in bytes, of the buffer we are allocating
    bufferInfo.size = verticesQuantity * sizeof(Vertex);
    //this buffer is going to be used as a Vertex Buffer
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

    //let the VMA library know that this data should be writeable by CPU, but also readable by GPU
    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    //allocate the buffer
    vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
        &vertexBuffer.buffer,
        &vertexBuffer.allocation,
        nullptr);

    //add the destruction of triangle mesh buffer to the deletion queue
    //_mainDeletionQueue.push_function([=]() {
    //    vmaDestroyBuffer(_allocator, mesh._vertexBuffer._buffer, mesh._vertexBuffer._allocation);
    //});

    void* data;
    vmaMapMemory(allocator, vertexBuffer.allocation, &data);

    memcpy(data, &vertices[0], vertices.Size() * sizeof(Vertex));

    vmaUnmapMemory(allocator, vertexBuffer.allocation);

    // Upload buffer data
    // glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // vertex positions
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //// vertex colours
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    //// vertex normals
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //// texture coordinates
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    //// weights for skinning
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

    //// joints for skinning
    //glEnableVertexAttribArray(5);
    //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, joints));

    //// glyphs for text
    //glEnableVertexAttribArray(6);
    //glVertexAttribIPointer(6, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, glyph));
}
