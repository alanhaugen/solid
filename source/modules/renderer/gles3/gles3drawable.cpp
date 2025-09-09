#include "gles3drawable.h"
#include "core/x-platform/typedefs.h"
#include <glm/gtc/type_ptr.hpp>

GLES3Drawable::GLES3Drawable(Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        GLES3Shader *shader_,
        Array<ITexture *> &textures_)
{
    for (unsigned int i = 0; i < textures_.Size(); i++)
    {
        GLES3Texture *gles2texture = dynamic_cast<GLES3Texture *>(textures_[i]);

        textures.Add(gles2texture);
    }

    if (textures.Size() != 0)
    {
        isTextured = true;
    }
    else
    {
        isTextured = false;
    }

    /*if (textures.Size() != 0)
    {
        // TODO: Fix, this code expects 6 textures => cubemap
        if (textures.Size() == 6)
        {
            textures[0]->Load(textures[0]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
            textures[0]->Load(textures[1]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
            textures[0]->Load(textures[2]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
            textures[0]->Load(textures[3]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
            textures[0]->Load(textures[4]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
            textures[0]->Load(textures[5]->name, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        }
        else
        {
            textures[0]->Load();
        }

        isTextured = true;
    }
    else
    {
        isTextured = false;
    }*/

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

    vao = 0;
    vbo = 0;
    ibo = 0;

    indicesQuantity  = indices.Size();
    verticesQuantity = vertices.Size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindVertexArray(vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (indices.Size() > 0)
    {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex colours
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // texture coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    // weights for skinning
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

    // joints for skinning
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, joints));

    // glyphs for text
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, glyph));

    glBindVertexArray(0);

    DeActivate();

    shader = shader_;
}

GLES3Drawable::~GLES3Drawable()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void GLES3Drawable::Activate(const glm::mat4& projViewMatrix)
{
    glUseProgram(shader->program);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Due to a bug on some cards, this is included

    if (textures.Empty() == false)
    {
        textures[0]->Activate();
    }

    glm::mat4 mvp;

    mvp = projViewMatrix * matrix;

    // Upload uniform block
    Uniform("MVP", static_cast<glm::mat4&>(mvp));
    Uniform("frame", static_cast<glm::uint>(frame));
    Uniform("verticesQuantity", static_cast<glm::uint>(verticesPerFrameQuantity));
    Uniform("colour", static_cast<glm::vec4>(uniforms.colour));
    Uniform("i_time", static_cast<glm::vec4>(uniforms.time.x));
    Uniform("i_index", static_cast<glm::vec4>(uniforms.index));
    Uniform("i_pos", static_cast<glm::vec4>(uniforms.pos));
    Uniform("i_scaleX", static_cast<glm::vec4>(uniforms.scaleX));
    Uniform("i_scaleY", static_cast<glm::vec4>(uniforms.scaleY));
    Uniform("i_width", static_cast<glm::vec4>(uniforms.width));
    Uniform("i_height", static_cast<glm::vec4>(uniforms.height));
    Uniform("i_totalWidth", static_cast<glm::vec4>(uniforms.totalWidth));
    Uniform("i_totalHeight", static_cast<glm::vec4>(uniforms.totalHeight));
    Uniform("i_screenWidth", static_cast<glm::vec4>(uniforms.screenWidth));
    Uniform("i_screenHeight", static_cast<glm::vec4>(uniforms.screenHeight));
    Uniform("i_flip", static_cast<glm::vec4>(uniforms.flip));
    Uniform("i_flipVertical", static_cast<glm::vec4>(uniforms.flipVertical));
    Uniform("i_colourTint", static_cast<glm::vec4>(uniforms.colourTint));

    if (animatedMatrices != NULL)
    {
        Uniform("animatedMatrices", static_cast<glm::mat4&>(animatedMatrices[0]), MAX_JOINTS);
    }

    if (isTextured)
    {
        Uniform("uEnableTexture", static_cast<bool>(true));
    }
    else
    {
        Uniform("uEnableTexture", static_cast<bool>(false));
    }
}

void GLES3Drawable::DeActivate()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

/*void GLES3Drawable::UploadBuffer(datatype, elements, data, offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, elements, datatype, GL_FALSE, sizeof(Vertex), (void*)offsetof(data, offset));
    index++;
}*/

int GLES3Drawable::GetUniform(String location)
{
    int uniformLocation;

    /*if (uniforms.Find(location) == NULL)
    {
        uniformLocation = glGetUniformLocation(shader->program, location.ToChar());
        uniforms.Insert(location, uniformLocation);
    }
    else
    {
        uniformLocation = uniforms.Find(location)->data_;
    }*/

    uniformLocation = glGetUniformLocation(shader->program, location.ToChar());
    return uniformLocation;
}

void GLES3Drawable::Uniform(String location, glm::f32 uniform)
{
    glUniform1f(GetUniform(location), uniform);
}

void GLES3Drawable::Uniform(String location, glm::vec2 uniform)
{
    glUniform2f(GetUniform(location), uniform.x, uniform.y);
}

void GLES3Drawable::Uniform(String location, glm::vec3 uniform)
{
    glUniform3f(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES3Drawable::Uniform(String location, glm::vec4 uniform)
{
    glUniform4f(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES3Drawable::Uniform(String location, glm::int32 uniform)
{
    glUniform1i(GetUniform(location), uniform);
}

void GLES3Drawable::Uniform(String location, glm::ivec2 uniform)
{
    glUniform2i(GetUniform(location), uniform.x, uniform.y);
}

void GLES3Drawable::Uniform(String location, glm::ivec3 uniform)
{
    glUniform3i(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES3Drawable::Uniform(String location, glm::ivec4 uniform)
{
    glUniform4i(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES3Drawable::Uniform(String location, glm::uint uniform)
{
    glUniform1ui(GetUniform(location), uniform);
}

void GLES3Drawable::Uniform(String location, glm::uvec2 uniform)
{
    glUniform2ui(GetUniform(location), uniform.x, uniform.y);
}

void GLES3Drawable::Uniform(String location, glm::uvec3 uniform)
{
    glUniform3ui(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES3Drawable::Uniform(String location, glm::uvec4 uniform)
{
    glUniform4ui(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES3Drawable::Uniform(String location, glm::f32 &uniform)
{
    glUniform1fv(GetUniform(location), 1, &uniform);
}

void GLES3Drawable::Uniform(String location, glm::vec2 &uniform)
{
    glUniform2fv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::vec3 &uniform)
{
    glUniform3fv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::vec4 &uniform)
{
    glUniform4fv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::int32 &uniform)
{
    glUniform1iv(GetUniform(location), 1, &uniform);
}

void GLES3Drawable::Uniform(String location, glm::ivec2 &uniform)
{
    glUniform2iv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::ivec3 &uniform)
{
    glUniform3iv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::ivec4 &uniform)
{
    glUniform4iv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::uint &uniform)
{
    glUniform1uiv(GetUniform(location), 1, &uniform);
}

void GLES3Drawable::Uniform(String location, glm::uvec2 &uniform)
{
    glUniform2uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::uvec3 &uniform)
{
    glUniform3uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::uvec4 &uniform)
{
    glUniform4uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES3Drawable::Uniform(String location, glm::mat2 &uniform)
{
    glUniformMatrix2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat3 &uniform)
{
    glUniformMatrix3fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat4 &uniform, int arraySize)
{
    glUniformMatrix4fv(GetUniform(location), arraySize, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat2x3 &uniform)
{
    glUniformMatrix4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat3x2 &uniform)
{
    glUniformMatrix3x2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat2x4 &uniform)
{
    glUniformMatrix2x4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat4x2 &uniform)
{
    glUniformMatrix4x2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat3x4 &uniform)
{
    glUniformMatrix3x4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES3Drawable::Uniform(String location, glm::mat4x3 &uniform)
{
    glUniformMatrix4x3fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}
