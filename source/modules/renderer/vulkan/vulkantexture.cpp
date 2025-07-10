#include "vulkantexture.h"
#include "vulkanrenderer.h"
#include "core/x-platform/locator.h"
#include "vk_mem_alloc.h"

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

    VulkanRenderer* renderer = dynamic_cast<VulkanRenderer*>(Locator::renderer);

    VkDeviceSize imageSize = width * height * 4;
    //renderer->CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    VkExtent3D imageExtent;
    imageExtent.width = static_cast<uint32_t>(width);
    imageExtent.height = static_cast<uint32_t>(height);
    imageExtent.depth = 1;

    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1; // Use mipmapping
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT; // Most important property
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    AllocatedImage newImage;

    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    //allocate and create the image
    vmaCreateImage(allocator, &imageInfo, &dimg_allocinfo, &newImage.image, &newImage.allocation, nullptr);

    stbi_image_free(img);

    /*engine.immediate_submit([&](VkCommandBuffer cmd) {
        VkImageSubresourceRange range;
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.levelCount = 1;
        range.baseArrayLayer = 0;
        range.layerCount = 1;

        VkImageMemoryBarrier imageBarrier_toTransfer = {};
        imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

        imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        imageBarrier_toTransfer.image = newImage._image;
        imageBarrier_toTransfer.subresourceRange = range;

        imageBarrier_toTransfer.srcAccessMask = 0;
        imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        //barrier the image into the transfer-receive layout
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);
    });*/

    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    copyRegion.bufferRowLength = 0;
    copyRegion.bufferImageHeight = 0;

    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageExtent = imageExtent;

    //copy the buffer into the image
    //vkCmdCopyBufferToImage(renderer->commandBuffer, stagingBuffer.buffer, newImage.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    VkImageMemoryBarrier imageBarrier_toReadable;// = imageBarrier_toTransfer;

    imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    //barrier the image into the shader readable layout
    vkCmdPipelineBarrier(renderer->commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toReadable);
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
