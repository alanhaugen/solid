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

    void CreateImageViews();
    void SetupDepthStencil();
    void CreateRenderPass();
    void CreateFramebuffers();

    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();
    void CreateFences();

    void CreateSemaphore(VkSemaphore *semaphore);

    VkPipelineShaderStageCreateInfo ShaderPipelineStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);

    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    VkBool32 GetSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat);

    void CreateImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage& image,
                     VkDeviceMemory& imageMemory);

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

    void AcquireNextImage();

    void QueueSubmit();
    void QueuePresent();

    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    uint32_t frameIndex;

    // Shaders
    bool LoadShader(const char* filePath, VkShaderModule* outShaderModule);

    // Graphics pipeline
    VkPipeline graphicsPipeline;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineLayout pipelineLayout;
    VkPipelineViewportStateCreateInfo viewportState;
    VkPipelineColorBlendStateCreateInfo colorBlending;

    float fade;

    VkPipeline CreateGraphicsPipeline(VkDevice device, VkRenderPass pass);

protected:
public:
    ~VulkanRenderer();

    void Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray);

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
