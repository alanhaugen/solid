#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "modules/renderer/null/nullrenderer.h"

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

class VulkanRenderer : public Renderer::NullRenderer
{
private:
    void CreateInstance(const char *windowTitle);
    bool SelectPhysicalDevice();

    void CreateImageViews();
    void Setup_DepthStencil();
    void Create_RenderPass();
    void Create_Framebuffers();

    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();
    void CreateFences();

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

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
    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D swapchainSize;

    std::vector<VkImage> swapchainImages;
    uint32_t swapchainImageCount;
    std::vector<VkImageView> swapchainImageViews;

    VkFormat depthFormat;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkRenderPass render_pass;

    std::vector<VkFramebuffer> swapchainFramebuffers;

    VkCommandPool commandPool;

    std::vector<VkCommandBuffer> commandBuffers;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderingFinishedSemaphore;

    std::vector<VkFence> fences;

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
    void SetupScreenAndCommand();
};

#endif
