#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "modules/renderer/null/nullrenderer.h"

class VulkanRenderer : public Renderer::NullRenderer
{
private:
    VkInstance instance;
    VkDevice device;

    VkSurfaceKHR surface;

    const std::vector<const char*> validationLayers;

    uint32_t graphics_QueueFamilyIndex;
    uint32_t present_QueueFamilyIndex;

    VkPhysicalDevice physicalDevice;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    //VkSwapchainKHR swapchain;
    //VkSurfaceCapabilitiesKHR surfaceCapabilities;

    bool isDeviceSuitable(VkPhysicalDevice device);

protected:
public:
    ~VulkanRenderer();

    std::vector<const char *> extensionNames; // Used by SDL to setup Vulkan

    bool Init(bool openFullscreened,
              const char *windowTitle,
              const unsigned int windowLength,
              const unsigned int windowHeight);
};

#endif
