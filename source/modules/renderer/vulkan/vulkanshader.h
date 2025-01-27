#ifndef VULKANSHADER_H
#define VULKANSHADER_H

#include "core/x-platform/locator.h"
#include "core/containers/array.h"
#include "core/containers/hash.h"
#include "modules/renderer/drawable.h"

class VulkanShader
{
public:
    VulkanShader();
    ~VulkanShader();

    unsigned int program;
    bool isCompiled;
    String name;

    bool LoadSpirV(int type, const char *filename);

    void Compile();

private:
};

#endif // VULKANSHADER_H
