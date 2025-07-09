#include "vulkantexture.h"
#include "core/x-platform/locator.h"

//#define STBI_ONLY_PNG
//#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

VulkanTexture::VulkanTexture()
{
}

VulkanTexture::VulkanTexture(String filePath, VkDevice device_, VkPhysicalDevice physicalDevice_, VmaAllocator allocator)
{
    device = device_;
    physicalDevice = physicalDevice_;

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

    VkFormat format;

    if (channels == TEXTURE_CHANNELS::RGB)
    {
        format = VK_FORMAT_R8G8B8_SRGB;
    }
    else
    {
        format = VK_FORMAT_R8G8B8A8_SRGB;
    }

    VkDeviceSize imageSize = width * height * 4;
    CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkExtent3D imageExtent;
    imageExtent.width = static_cast<uint32_t>(width);
    imageExtent.height = static_cast<uint32_t>(height);
    imageExtent.depth = 1;

    VkImageCreateInfo dimg_info;// = vkinit::image_create_info(image_format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);

    AllocatedImage newImage;

    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    //allocate and create the image
    vmaCreateImage(allocator, &dimg_info, &dimg_allocinfo, &newImage.image, &newImage.allocation, nullptr);

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
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);
}
