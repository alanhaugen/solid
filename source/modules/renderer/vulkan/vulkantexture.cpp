#include "vulkantexture.h"
#include "core/x-platform/locator.h"

//#define STBI_ONLY_PNG
//#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

VulkanTexture::VulkanTexture()
{
}

VulkanTexture::VulkanTexture(String filePath)
{
    IFile *file;

    if (filePath == "")
    {
        file = Locator::filesystem->Open(filePath);
    }
    else
    {
        file = Locator::filesystem->Open(filePath);
    }

    unsigned char *img = stbi_load_from_memory((const unsigned char*)file->Read(), file->Size(), &width, &height, &channels, 0);

    if (channels == TEXTURE_CHANNELS::RGB)
    {
        //format = GL_RGB;
    }
    else
    {
        //format = GL_RGBA;
    }

    stbi_image_free(img);
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
