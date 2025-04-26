#include "gles2drawable.h"
#include <glm/gtc/type_ptr.hpp>

GLES2Drawable::GLES2Drawable(Array<IDrawable::Vertex> &vertices,
        Array<unsigned int> &indices,
        GLES2Shader *shader_,
        Array<ITexture *> &textures_)
{
    shader = shader_;

    for (unsigned int i = 0; i < textures_.Size(); i++)
    {
        GLES2Texture *gles2texture = dynamic_cast<GLES2Texture *>(textures_[i]);

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

    if (textures.Size() != 0)
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

    vbo = 0;
    ibo = 0;

    indicesQuantity  = indices.Size();
    verticesQuantity = vertices.Size();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.Size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (indices.Size() > 0)
    {
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);
    }

    glBindAttribLocation(shader->program, 0, "vVertex");
    glBindAttribLocation(shader->program, 1, "vColor");
    glBindAttribLocation(shader->program, 2, "vNormal");
    glBindAttribLocation(shader->program, 3, "vTexcoord");
    glBindAttribLocation(shader->program, 4, "vWeights");
    glBindAttribLocation(shader->program, 5, "vJoints");
    glBindAttribLocation(shader->program, 6, "vGlyph");

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // vertex colours
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // vertex normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // texture coordinates
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    // weights for skinning
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));

    // joints for skinning
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, joints));

    // glyphs for text
    glVertexAttribIPointer(6, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, glyph));

    DeActivate();
}

GLES2Drawable::~GLES2Drawable()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void GLES2Drawable::Activate(const glm::mat4& projViewMatrix)
{
    glUseProgram(shader->program);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    if (textures.Empty() == false)
    {
        textures[0]->Activate();
    }

    glm::mat4 mvp;

    mvp = projViewMatrix * matrix;

    // for each uniform

    if (uniformData.empty == false)
    {
        for (unsigned int i = 0; i < (*uniformData)->Size(); i++)
        {
            UniformData *data = *(*uniformData)->array[i];

            // enum type { f32, vec2, vec3, vec4, int32, ivec2, ivec3, ivec4, uint, uvec2, uvec3, uvec4, mat2, mat3, mat4, mat2x3, mat3x2, mat2x4, mat4x2, mat3x4, mat4x3 };
            if (data->dataType == UniformData::f32)
            {
                glm::f32 *p = static_cast<glm::f32*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::f32>(*p));
            }
            else if (data->dataType == UniformData::vec2)
            {
                glm::vec2 *p = static_cast<glm::vec2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::vec2>(*p));
            }
            else if (data->dataType == UniformData::vec3)
            {
                glm::vec3 *p = static_cast<glm::vec3*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::vec3>(*p));
            }
            else if (data->dataType == UniformData::vec4)
            {
                glm::vec4 *p = static_cast<glm::vec4*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::vec4>(*p));
            }
            else if (data->dataType == UniformData::int32)
            {
                glm::int32 *p = static_cast<glm::int32*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::int32>(*p));
            }
            else if (data->dataType == UniformData::ivec2)
            {
                glm::ivec2 *p = static_cast<glm::ivec2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::ivec2>(*p));
            }
            else if (data->dataType == UniformData::ivec4)
            {
                glm::ivec4 *p = static_cast<glm::ivec4*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::ivec4>(*p));
            }
            else if (data->dataType == UniformData::uint)
            {
                glm::uint *p = static_cast<glm::uint*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::uint>(*p));
            }
            else if (data->dataType == UniformData::uvec2)
            {
                glm::uvec2 *p = static_cast<glm::uvec2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::uvec2>(*p));
            }
            else if (data->dataType == UniformData::uvec3)
            {
                glm::uvec3 *p = static_cast<glm::uvec3*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::uvec3>(*p));
            }
            else if (data->dataType == UniformData::uvec4)
            {
                glm::uvec4 *p = static_cast<glm::uvec4*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::uvec4>(*p));
            }
            else if (data->dataType == UniformData::mat2)
            {
                glm::mat2 *p = static_cast<glm::mat2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat2&>(*p));
            }
            else if (data->dataType == UniformData::mat3)
            {
                glm::mat3 *p = static_cast<glm::mat3*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat3&>(*p));
            }
            else if (data->dataType == UniformData::mat4)
            {
                glm::mat4 *p = static_cast<glm::mat4*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat4&>(*p));
            }
            else if (data->dataType == UniformData::mat2x3)
            {
                glm::mat2x3 *p = static_cast<glm::mat2x3*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat2x3&>(*p));
            }
            else if (data->dataType == UniformData::mat3x2)
            {
                glm::mat3x2 *p = static_cast<glm::mat3x2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat3x2&>(*p));
            }
            else if (data->dataType == UniformData::mat4x2)
            {
                glm::mat4x2 *p = static_cast<glm::mat4x2*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat4x2&>(*p));
            }
            else if (data->dataType == UniformData::mat3x4)
            {
                glm::mat3x4 *p = static_cast<glm::mat3x4*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat3x4&>(*p));
            }
            else if (data->dataType == UniformData::mat4x3)
            {
                glm::mat4x3 *p = static_cast<glm::mat4x3*>(data->data); // void* dereferencing
                Uniform(data->handle, static_cast<glm::mat4x3&>(*p));
            }
            else
            {
                LogWarning("Datatype not supported");
            }
        }

        if ((*uniformData)->Size() == 0)
        {
            Uniform("colour", static_cast<glm::vec4>(colorTint));
        }
    }

    Uniform("MVP", static_cast<glm::mat4&>(mvp));

    Uniform("frame", static_cast<glm::uint>(frame));
    Uniform("verticesQuantity", static_cast<glm::uint>(verticesPerFrameQuantity));

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

void GLES2Drawable::DeActivate()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

/*void GLES2Drawable::UploadBuffer(datatype, elements, data, offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, elements, datatype, GL_FALSE, sizeof(Vertex), (void*)offsetof(data, offset));
    index++;
}*/

int GLES2Drawable::GetUniform(String location)
{
    int uniformLocation;

    if (uniforms.Find(location) == NULL)
    {
        uniformLocation = glGetUniformLocation(shader->program, location.ToChar());
        uniforms.Insert(location, uniformLocation);
    }
    else
    {
        uniformLocation = uniforms.Find(location)->data_;
    }

    return uniformLocation;
}

void GLES2Drawable::Uniform(String location, glm::f32 uniform)
{
    glUniform1f(GetUniform(location), uniform);
}

void GLES2Drawable::Uniform(String location, glm::vec2 uniform)
{
    glUniform2f(GetUniform(location), uniform.x, uniform.y);
}

void GLES2Drawable::Uniform(String location, glm::vec3 uniform)
{
    glUniform3f(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES2Drawable::Uniform(String location, glm::vec4 uniform)
{
    glUniform4f(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES2Drawable::Uniform(String location, glm::int32 uniform)
{
    glUniform1i(GetUniform(location), uniform);
}

void GLES2Drawable::Uniform(String location, glm::ivec2 uniform)
{
    glUniform2i(GetUniform(location), uniform.x, uniform.y);
}

void GLES2Drawable::Uniform(String location, glm::ivec3 uniform)
{
    glUniform3i(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES2Drawable::Uniform(String location, glm::ivec4 uniform)
{
    glUniform4i(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES2Drawable::Uniform(String location, glm::uint uniform)
{
    glUniform1ui(GetUniform(location), uniform);
}

void GLES2Drawable::Uniform(String location, glm::uvec2 uniform)
{
    glUniform2ui(GetUniform(location), uniform.x, uniform.y);
}

void GLES2Drawable::Uniform(String location, glm::uvec3 uniform)
{
    glUniform3ui(GetUniform(location), uniform.x, uniform.y, uniform.z);
}

void GLES2Drawable::Uniform(String location, glm::uvec4 uniform)
{
    glUniform4ui(GetUniform(location), uniform.x, uniform.y, uniform.z, uniform.w);
}

void GLES2Drawable::Uniform(String location, glm::f32 &uniform)
{
    glUniform1fv(GetUniform(location), 1, &uniform);
}

void GLES2Drawable::Uniform(String location, glm::vec2 &uniform)
{
    glUniform2fv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::vec3 &uniform)
{
    glUniform3fv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::vec4 &uniform)
{
    glUniform4fv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::int32 &uniform)
{
    glUniform1iv(GetUniform(location), 1, &uniform);
}

void GLES2Drawable::Uniform(String location, glm::ivec2 &uniform)
{
    glUniform2iv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::ivec3 &uniform)
{
    glUniform3iv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::ivec4 &uniform)
{
    glUniform4iv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::uint &uniform)
{
    glUniform1uiv(GetUniform(location), 1, &uniform);
}

void GLES2Drawable::Uniform(String location, glm::uvec2 &uniform)
{
    glUniform2uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::uvec3 &uniform)
{
    glUniform3uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::uvec4 &uniform)
{
    glUniform4uiv(GetUniform(location), 1, &uniform[0]);
}

void GLES2Drawable::Uniform(String location, glm::mat2 &uniform)
{
    glUniformMatrix2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat3 &uniform)
{
    glUniformMatrix3fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat4 &uniform, int arraySize)
{
    glUniformMatrix4fv(GetUniform(location), arraySize, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat2x3 &uniform)
{
    glUniformMatrix4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat3x2 &uniform)
{
    glUniformMatrix3x2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat2x4 &uniform)
{
    glUniformMatrix2x4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat4x2 &uniform)
{
    glUniformMatrix4x2fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat3x4 &uniform)
{
    glUniformMatrix3x4fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}

void GLES2Drawable::Uniform(String location, glm::mat4x3 &uniform)
{
    glUniformMatrix4x3fv(GetUniform(location), 1, GL_FALSE, &uniform[0][0]);
}
