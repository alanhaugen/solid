#include "vulkantexture.h"

//#define STBI_ONLY_PNG
//#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

VulkanTexture::VulkanTexture()
{
}

VulkanTexture::VulkanTexture(String filePath)
{
}

VulkanTexture::VulkanTexture(String front, String back, String top, String bottom, String left, String right)
{
}

U8 VulkanTexture::At(unsigned int x, unsigned int y, U8 data)
{

}

U8 VulkanTexture::At(unsigned int index, U8 data)
{

}

void VulkanTexture::ReUpload()
{

}

void VulkanTexture::ReUpload(String filePath)
{

}

VulkanTexture::~VulkanTexture()
{
}
