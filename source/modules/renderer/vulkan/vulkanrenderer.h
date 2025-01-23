#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "modules/renderer/null/nullrenderer.h"

class VulkanRenderer : public Renderer::NullRenderer
{
private:
    void CreateInstance(const char *windowTitle);
    bool SelectPhysicalDevice();

    VkDevice device;
    VkImage image;

    const std::vector<const char*> validationLayers;

    uint32_t graphics_QueueFamilyIndex;
    uint32_t present_QueueFamilyIndex;

    VkPhysicalDevice physicalDevice;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    bool isDeviceSuitable(VkPhysicalDevice device);

    VkSwapchainKHR swapchain;
    VkSurfaceCapabilitiesKHR surfaceCapabilities;

    VkCommandBuffer commandBuffer;

protected:
public:
    ~VulkanRenderer();

    // Used by SDL to setup Vulkan
    std::vector<const char *> extensionNames;
    VkInstance instance;
    VkSurfaceKHR surface;

    // Init renderer
    bool Init(bool openFullscreened,
              const char *windowTitle,
              const unsigned int windowLength,
              const unsigned int windowHeight);

    void SelectQueueFamily();
    bool CreateDevice();
    void CreateSwapChain(int width, int height);
};

#endif
