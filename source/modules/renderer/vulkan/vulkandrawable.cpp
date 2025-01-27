#include "vulkandrawable.h"

VulkanDrawable::VulkanDrawable(Array<IDrawable::Vertex> &vertices,
                               Array<unsigned int> &indices,
                               VulkanShader* shader_,
                               Array<ITexture *> &textures)
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
