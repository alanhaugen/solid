#ifndef VULKANTEXTURE_H
#define VULKANTEXTURE_H

#include "vk_mem_alloc.h"
#include "modules/renderer/texture.h"

class VulkanTexture : public ITexture
{
public:
    VulkanTexture();
    VulkanTexture(String filename, VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator);
    VulkanTexture(String front, String back, String top, String bottom, String left, String right);

    U8 At(unsigned int x, unsigned int y, U8 data);
    U8 At(unsigned int index, U8 data);

    void ReUpload();
    void ReUpload(String filePath);

    ~VulkanTexture();

private:
    struct AllocatedImage {
        VkImage image;
        VmaAllocation allocation;
    };

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif // VULKANTEXTURE_H
