#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "modules/renderer/null/nullrenderer.h"
#include "core/containers/array.h"
#include "vulkandrawable.h"
#include "vulkanshader.h"
#include "vk_mem_alloc.h"

class VulkanRenderer : public Renderer::NullRenderer
{
private:
    struct UploadContext {
        VkFence uploadFence;
        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;
    };

    UploadContext uploadContext;

    /*// Depth buffer
    struct AllocatedImage {
        VkImage image;
        VmaAllocation allocation;
    };

    VkImageView depthImageView;
    AllocatedImage depthImage;

    VkFormat depthFormat; // The format for the depth image*/

    VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
    VkSubmitInfo submitInfo(VkCommandBuffer* cmd);

    AllocatedBuffer CreateBuffer(size_t allocSize,
                                 VkBufferUsageFlags usage,
                                 VmaMemoryUsage memoryUsage);

    void SetupUploadContext();
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
    void SetupVMA();

    void SetupDescriptionPool();
    void SetupDescriptorSets();

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

    VkDescriptorSet descriptor;
    AllocatedBuffer uniformBuffer;

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

    VkPhysicalDeviceProperties gpuProperties;

    // Shaders
    bool LoadShader(const char* filePath, VkShaderModule* outShaderModule);

    // Graphics pipeline
    VkShaderModule triangleFragShader;
    VkShaderModule triangleVertShader;

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineRasterizationStateCreateInfo rasterizer;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo multisampling;
    VkPipelineViewportStateCreateInfo viewportState;
    VkPipelineColorBlendStateCreateInfo colorBlending;

    VkPipeline CreateGraphicsPipeline(VkDevice device, VkRenderPass pass,
                                      const char *fragShaderPath, const char *vertShaderPath,
                                      VulkanDrawable* drawable,
                                      int topology);

    // Classic solid book-keeping
    LinkedList<VulkanDrawable*> drawables;

    // Vulkan Memory Allocator (VMA) from the AMD GPUOpen project (many thanks to AMD)
    VmaAllocator allocator;

    // Descriptor Set Pool (to send uniforms to the GPU)
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout setLayout;

protected:
public:
    ~VulkanRenderer();

    void Render(const Array<glm::mat4>& projViewMatrixArray, const Array<glm::vec4>& viewBoundsArray);
    ITexture* CreateTexture(String filename);
    ITexture* CreateTexture(String front, String back, String top, String bottom, String left, String right);
    ITexture* CreateTexture(int width, int height);

    // Used by SDL to setup Vulkan
    std::vector<const char *> extensionNames;
    VkInstance instance;
    VkSurfaceKHR surface;

    size_t PadUniformBufferSize(size_t originalSize);

    // Init renderer
    bool Init(bool openFullscreened,
              const char *windowTitle,
              const unsigned int windowLength,
              const unsigned int windowHeight);

    void SelectQueueFamily();
    bool CreateDevice();
    void CreateSwapChain(int width, int height);
    bool SetupScreenAndCommand();

    IDrawable *CreateDrawable(Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            Array<ITexture *> textures,
            int topology = DRAW_TRIANGLES);
    IDrawable *CreateDrawable(
            Array<IDrawable::Vertex> &vertices,
            Array<unsigned int> &indices,
            Array<String> &shaders,
            ITexture* texture,
            int topology = DRAW_TRIANGLES);
    void RemoveDrawable(IDrawable *drawable);
};

#endif
