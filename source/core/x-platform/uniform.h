#ifndef UNIFORM_H
#define UNIFORM_H

#include "glm/glm.hpp"
#include "core/x-platform/string.h"

class UniformData
{
private:
public:
    enum type { f32, vec2, vec3, vec4, int32, ivec2, ivec3, ivec4, uint, uvec2, uvec3, uvec4, mat2, mat3, mat4, mat2x3, mat3x2, mat2x4, mat4x2, mat3x4, mat4x3 };

    int dataType;
    String handle;
    void *data;

    ~UniformData()
    {
        if (dataType == f32)
        {
            glm::f32 *deleteData = static_cast<glm::f32*>(data);
            delete deleteData;
        }
        else if (dataType == vec2)
        {
            glm::vec2 *deleteData = static_cast<glm::vec2*>(data);
            delete deleteData;
        }
        else if (dataType == vec4)
        {
            glm::vec4 *deleteData = static_cast<glm::vec4*>(data);
            delete deleteData;
        }
        else if (dataType == int32)
        {
            glm::int32 *deleteData = static_cast<glm::int32*>(data);
            delete deleteData;
        }
        else if (dataType == ivec2)
        {
            glm::ivec2 *deleteData = static_cast<glm::ivec2*>(data);
            delete deleteData;
        }
        else if (dataType == ivec3)
        {
            glm::ivec3 *deleteData = static_cast<glm::ivec3*>(data);
            delete deleteData;
        }
        else if (dataType == ivec4)
        {
            glm::ivec4 *deleteData = static_cast<glm::ivec4*>(data);
            delete deleteData;
        }
        else if (dataType == uint)
        {
            glm::uint *deleteData = static_cast<glm::uint*>(data);
            delete deleteData;
        }
        else if (dataType == uvec2)
        {
            glm::uvec2 *deleteData = static_cast<glm::uvec2*>(data);
            delete deleteData;
        }
        else if (dataType == uvec3)
        {
            glm::uvec3 *deleteData = static_cast<glm::uvec3*>(data);
            delete deleteData;
        }
        else if (dataType == uvec4)
        {
            glm::uvec4 *deleteData = static_cast<glm::uvec4*>(data);
            delete deleteData;
        }
        else if (dataType == mat2)
        {
            glm::mat2 *deleteData = static_cast<glm::mat2*>(data);
            delete deleteData;
        }
        else if (dataType == mat3)
        {
            glm::mat3 *deleteData = static_cast<glm::mat3*>(data);
            delete deleteData;
        }
        else if (dataType == mat4)
        {
            glm::mat4 *deleteData = static_cast<glm::mat4*>(data);
            delete deleteData;
        }
        else if (dataType == mat2x3)
        {
            glm::mat2x3 *deleteData = static_cast<glm::mat2x3*>(data);
            delete deleteData;
        }
        else if (dataType == mat3x2)
        {
            glm::mat3x2 *deleteData = static_cast<glm::mat3x2*>(data);
            delete deleteData;
        }
        else if (dataType == mat2x4)
        {
            glm::mat2x4 *deleteData = static_cast<glm::mat2x4*>(data);
            delete deleteData;
        }
        else if (dataType == mat4x2)
        {
            glm::mat4x2 *deleteData = static_cast<glm::mat4x2*>(data);
            delete deleteData;
        }
        else if (dataType == mat3x4)
        {
            glm::mat3x4 *deleteData = static_cast<glm::mat3x4*>(data);
            delete deleteData;
        }
        else if (dataType == mat4x3)
        {
            glm::mat4x3 *deleteData = static_cast<glm::mat4x3*>(data);
            delete deleteData;
        }

        data = NULL;
    }

    UniformData(String location, glm::f32 &uniform)
    {
        handle = location;
        data = new glm::f32(uniform);
        dataType = f32;
    }

    UniformData(String location, glm::vec2 &uniform)
    {
        handle = location;
        data = new glm::vec2(uniform);
        dataType = vec2;
    }

    UniformData(String location, glm::vec3 &uniform)
    {
        handle = location;
        data = new glm::vec3(uniform);
        dataType = vec3;
    }

    UniformData(String location, glm::vec4 &uniform)
    {
        handle = location;
        data = new glm::vec4(uniform);
        dataType = vec4;
    }

    UniformData(String location, glm::int32 &uniform)
    {
        handle = location;
        data = new glm::int32(uniform);
        dataType = int32;
    }

    UniformData(String location, glm::ivec2 &uniform)
    {
        handle = location;
        data = new glm::ivec2(uniform);
        dataType = ivec2;
    }

    UniformData(String location, glm::ivec3 &uniform)
    {
        handle = location;
        data = new glm::ivec3(uniform);
        dataType = ivec3;
    }

    UniformData(String location, glm::ivec4 &uniform)
    {
        handle = location;
        data = new glm::ivec4(uniform);
        dataType = ivec4;
    }

    UniformData(String location, glm::uint &uniform)
    {
        handle = location;
        data = new glm::uint(uniform);
        dataType = uint;
    }

    UniformData(String location, glm::uvec2 &uniform)
    {
        handle = location;
        data = new glm::uvec2(uniform);
        dataType = uvec2;
    }

    UniformData(String location, glm::uvec3 &uniform)
    {
        handle = location;
        data = new glm::uvec3(uniform);
        dataType = uvec3;
    }

    UniformData(String location, glm::uvec4 &uniform)
    {
        handle = location;
        data = new glm::uvec4(uniform);
        dataType = uvec4;
    }

    UniformData(String location, glm::f32 *uniform)
    {
        handle = location;
        data = uniform;
        dataType = f32;
    }

    UniformData(String location, glm::mat2 &uniform)
    {
        handle = location;
        data = new glm::mat2(uniform);
        dataType = mat2;
    }

    UniformData(String location, glm::mat3 &uniform)
    {
        handle = location;
        data = new glm::mat3(uniform);
        dataType = mat3;
    }

    UniformData(String location, glm::mat4 &uniform)
    {
        handle = location;
        data = new glm::mat4(uniform);
        dataType = mat4;
    }

    UniformData(String location, glm::mat2x3 &uniform)
    {
        handle = location;
        data = new glm::mat2x3(uniform);
        dataType = mat2x3;
    }

    UniformData(String location, glm::mat3x2 &uniform)
    {
        handle = location;
        data = new glm::mat3x2(uniform);
        dataType = mat3x2;
    }

    UniformData(String location, glm::mat2x4 &uniform)
    {
        handle = location;
        data = new glm::mat2x4(uniform);
        dataType = mat2x4;
    }

    UniformData(String location, glm::mat4x2 &uniform)
    {
        handle = location;
        data = new glm::mat4x2(uniform);
        dataType = mat4x2;
    }

    UniformData(String location, glm::mat3x4 &uniform)
    {
        handle = location;
        data = new glm::mat3x4(uniform);
        dataType = mat3x4;
    }

    UniformData(String location, glm::mat4x3 &uniform)
    {
        handle = location;
        data = new glm::mat4x3(uniform);
        dataType = mat4x3;
    }
};

class Uniform
{
public:
    Uniform();
};

#endif // UNIFORM_H
