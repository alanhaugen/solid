#include "vulkanshader.h"

VulkanShader::VulkanShader()
{
    isCompiled = false;
}

VulkanShader::~VulkanShader()
{
}

void VulkanShader::Compile()
{
}

bool VulkanShader::LoadSpirV(int type, const char *filename)
{
    LogError("Failed to create new shader object");
}
