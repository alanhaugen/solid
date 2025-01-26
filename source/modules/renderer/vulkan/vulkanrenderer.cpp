#include "vulkanrenderer.h"
#include "vulkandrawable.h"
#include <set>

// Thanks to https://gist.github.com/YukiSnowy/dc31f47448ac61dd6aedee18b5d53858 and https://vkguide.dev

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

void VulkanRenderer::CreateInstance(const char *windowTitle)
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = windowTitle;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

    // pEngineName and engineVersion contain the name and version,
    // respectively, of the engine or middleware that your application is based on
    appInfo.pEngineName = "Solid";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Setup info for instance
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo; // appInfo from above set here
    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    instanceCreateInfo.enabledExtensionCount = extensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

    vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
}

bool VulkanRenderer::SelectPhysicalDevice()
{
    // We will try to find a supported vulkan device
    physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    // Let's see if we have any GPUs
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        // No GPUs found, return failure
        return false;
    }

    // We have GPUs, select one of them
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
    {
        // No GPUs selected, return failure
        return false;
    }

    return true;
}

void VulkanRenderer::CreateImageViews()
{
    // A swapchain is a OS/windowing provided structure with some images we can draw to and then display
    // on the screen. Swapchains are not in the core Vulkan spec as they are optional, and often unique
    // to the different platforms. If you are going to use Vulkan for compute shader calculations, or for
    // offline rendering, you do not need to setup a swapchain.
    swapchainImageViews.resize(swapchainImages.size());

    for (uint32_t i = 0; i < swapchainImages.size(); i++)
    {
        swapchainImageViews[i] = CreateImageView(swapchainImages[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkBool32 VulkanRenderer::GetSupportedDepthFormat(VkPhysicalDevice physicalDevice, VkFormat *depthFormat)
{
    std::vector<VkFormat> depthFormats = {
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D16_UNORM
    };

    for (auto& format : depthFormats)
    {
        VkFormatProperties formatProps;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProps);
        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            *depthFormat = format;
            return true;
        }
    }

    return false;
}

void VulkanRenderer::SetupDepthStencil()
{
    VkBool32 validDepthFormat = GetSupportedDepthFormat(physicalDevice, &depthFormat);

    // We will ask for the image tiling OPTIMAL,
    // which means that we allow the GPU to shuffle the data however it sees fit
    CreateImage(swapchainSize.width, swapchainSize.height,
                VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                depthImage, depthImageMemory);

    depthImageView = CreateImageView(depthImage, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkImageView VulkanRenderer::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    // With the image allocated, we create an imageview to pair with it.
    // In Vulkan, you need a imageview to access images. This is generally a thin wrapper over the image itself
    // that lets you do things like limit access to only 1 mipmap
    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;

    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

uint32_t VulkanRenderer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanRenderer::CreateImage(uint32_t width,
                                 uint32_t height,
                                 VkFormat format,
                                 VkImageTiling tiling,
                                 VkImageUsageFlags usage, // *important*
                                 VkMemoryPropertyFlags properties,
                                 VkImage& image,
                                 VkDeviceMemory& imageMemory) // *important*
{
    // When creating the image itself, we need to send the image info and an alloc info to VMA.
    // VMA will do the vulkan create calls for us and directly give us the vulkan image. The
    // interesting thing in here is Usage and the required memory flags.
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1; // Use mipmapping
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage; // Most important property
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    // In Vulkan, there are multiple memory regions we can allocate images and buffers from.
    // PC implementations with dedicated GPUs will generally have a cpu ram region, a GPU Vram region,
    // and a “upload heap” which is a special region of gpu vram that allows CPU writes.
    // If you have resizable bar enabled, the upload heap can be the entire GPU vram.
    // Else it will be much smaller, generally only 256 megabytes.
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, imageMemory, 0);
}

void VulkanRenderer::CreateRenderPass()
{
    std::vector<VkAttachmentDescription> attachments(2);

    attachments[0].format = surfaceFormat.format;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    attachments[1].format = depthFormat;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorReference = {};
    colorReference.attachment = 0;
    colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthReference = {};
    depthReference.attachment = 1;
    depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;
    subpassDescription.inputAttachmentCount = 0;
    subpassDescription.pInputAttachments = nullptr;
    subpassDescription.preserveAttachmentCount = 0;
    subpassDescription.pPreserveAttachments = nullptr;
    subpassDescription.pResolveAttachments = nullptr;

    std::vector<VkSubpassDependency> dependencies(1);

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    vkCreateRenderPass(device, &renderPassInfo, nullptr, &render_pass);
}

void VulkanRenderer::CreateFramebuffers()
{
    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
        std::vector<VkImageView> attachments(2);
        attachments[0] = swapchainImageViews[i];
        attachments[1] = depthImageView;

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = render_pass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchainSize.width;
        framebufferInfo.height = swapchainSize.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VulkanRenderer::CreateCommandPool()
{
    VkResult result;

    VkCommandPoolCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = graphics_QueueFamilyIndex;
    vkCreateCommandPool(device, &createInfo, nullptr, &commandPool);
}

void VulkanRenderer::CreateCommandBuffers()
{
    VkResult result;

    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = swapchainImageCount;

    commandBuffers.resize(swapchainImageCount);
    vkAllocateCommandBuffers(device, &allocateInfo, commandBuffers.data());
}

void VulkanRenderer::CreateSemaphores()
{
    CreateSemaphore(&imageAvailableSemaphore);
    CreateSemaphore(&renderingFinishedSemaphore);
}

void VulkanRenderer::CreateFences()
{
    // This is used for GPU -> CPU communication. A lot of Vulkan operations, such as vkQueueSubmit
    // allow an optional fence parameter. If this is set, we can know from the CPU if the GPU has finished
    // these operations. We will use it to sync the main loop in the CPU with the GPU. A fence will be signaled
    // once submitted as part of a command, and then we can use VkWaitForFences to have the CPU stop until those
    // commands have executed.
    uint32_t i;
    fences.resize(swapchainImageCount);
    for(i = 0; i < swapchainImageCount; i++)
    {
        VkResult result;

        VkFenceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &createInfo, nullptr, &fences[i]);
    }
}

void VulkanRenderer::CreateSemaphore(VkSemaphore *semaphore)
{
    VkResult result;

    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(device, &createInfo, nullptr, semaphore);
}

// It is the queues that perform the work that the application requests.
// Let's select graphics and presentation queues
void VulkanRenderer::SelectQueueFamily()
{
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    uint32_t queueFamilyCount;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    int graphicIndex = -1;
    int presentIndex = -1;

    int i = 0;

    for (const auto& queueFamily : queueFamilyProperties)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicIndex = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (queueFamily.queueCount > 0 && presentSupport)
        {
            presentIndex = i;
        }

        if (graphicIndex != -1 && presentIndex != -1)
        {
            break;
        }

        i++;
    }

    graphics_QueueFamilyIndex = graphicIndex;
    present_QueueFamilyIndex  = presentIndex;
}

bool VulkanRenderer::CreateDevice()
{
    // Let us request the extension VK_KHR_SWAPCHAIN for backbuffer support
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    const float queue_priority[] = { 1.0f };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { graphics_QueueFamilyIndex, present_QueueFamilyIndex };

    // Setup queues
    float queuePriority = queue_priority[0];
    for(int queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphics_QueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // https://en.wikipedia.org/wiki/Anisotropic_filtering
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    // Setup info for device and create it
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = queueCreateInfos.size();
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();

    // Create the device based on physical device and queue requests
    vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);

    // Get the requested device queues
    vkGetDeviceQueue(device, graphics_QueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, present_QueueFamilyIndex, 0, &presentQueue);

    return true;
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
    return true;
}

void VulkanRenderer::AcquireNextImage()
{
    // vkAcquireNextImageKHR will request the image index from the swapchain, and if the swapchain doesn’t have any
    // image we can use, it will block the thread with a maximum for the timeout set, which will be 1 second.
    vkAcquireNextImageKHR(device,
                          swapchain,
                          UINT64_MAX, // Wait time (infinite)
                          imageAvailableSemaphore,
                          VK_NULL_HANDLE,
                          &frameIndex);

    // Wait until the gpu has finished rendering the last frame. Timeout forever
    vkWaitForFences(device, 1, &fences[frameIndex], VK_FALSE, UINT64_MAX);
    vkResetFences(device, 1, &fences[frameIndex]);

    commandBuffer = commandBuffers[frameIndex];
    image = swapchainImages[frameIndex];
}

void VulkanRenderer::QueueSubmit()
{
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask = &waitStage;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderingFinishedSemaphore;

    // Submits a sequence of semaphores or command buffers to a queue
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, fences[frameIndex]);
}

void VulkanRenderer::QueuePresent()
{
    VkPresentInfoKHR presentInfo = {};

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderingFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain;
    presentInfo.pImageIndices = &frameIndex;

    vkQueuePresentKHR(presentQueue, &presentInfo);
    vkQueueWaitIdle(presentQueue);
}

VulkanRenderer::~VulkanRenderer()
{
    // Cleanly shut down the Vulkan system by waiting for a device to complete work submitted by
    // the application, destroying the device handles, and finally destroying the instance handle
    vkDeviceWaitIdle(device);

    // Delete command pool
    vkDestroyCommandPool(device, commandPool, nullptr);

    // Destroy fences
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        vkDestroyFence(device, fences[i], nullptr);
    }

    // Destroy semaphores
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(device, renderingFinishedSemaphore, nullptr);

    // Destroy spapchain
    vkDestroySwapchainKHR(device, swapchain, nullptr);

    // Destroy swapchain resources
    for (int i = 0; i < swapchainImageViews.size(); i++)
    {
        vkDestroyImageView(device, swapchainImageViews[i], nullptr);
    }

    // Destroy surface
    vkDestroySurfaceKHR(instance, surface, nullptr);

    // Destroy device and then instance
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::Render(const Array<glm::mat4> &projViewMatrixArray, const Array<glm::vec4> &viewBoundsArray)
{
    AcquireNextImage();

    // Now that we are sure that the commands finished executing, we can safely
    // reset the command buffer to begin recording again.
    vkResetCommandBuffer(commandBuffer, 0);

    // Begin a new command buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkClearColorValue clear_color = { 1.0f, 0.0f, 0.0f, 1.0f };
    VkClearDepthStencilValue clear_depth_stencil = { 1.0f, 0 };

    PreRender();

    // Time to begin the rendering commands. For that, we are going to reset the command buffer for this frame,
    // and begin it again. We will need to use another one of the initializer functions.
    VkRenderPassBeginInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass        = render_pass;
    render_pass_info.framebuffer       = swapchainFramebuffers[frameIndex];
    render_pass_info.renderArea.offset = { 0, 0 };
    render_pass_info.renderArea.extent = swapchainSize;
    render_pass_info.clearValueCount   = 1;

    std::vector<VkClearValue> clearValues(2);
    clearValues[0].color = clear_color;
    clearValues[1].depthStencil = clear_depth_stencil;

    render_pass_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
    render_pass_info.pClearValues = clearValues.data();

    // In Vulkan, all of the rendering happens inside a VkRenderPass (NOTE: For Vulkan 1.3, this has changed)
    vkCmdBeginRenderPass(commandBuffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    // Rendering commands go here

    /*for (unsigned i = 0; i<projViewMatrixArray.Size(); i++)
    {
    }*/

    // End render pass (Remember, we are currently targeting Vulkan before Vulkan 1.3)
    vkCmdEndRenderPass(commandBuffer);

    // Finalize the command buffer (we can no longer add commands, but it can now be executed)
    vkEndCommandBuffer(commandBuffer);

    // Submit command buffer commands. Note we are not using vkQueueSubmit2 since we are using Vulkan 1.0.
    QueueSubmit();

    // Put the graphics onto the screen
    QueuePresent();

    PostRender();
}

bool VulkanRenderer::Init(bool openFullscreened,
                          const char *windowTitle,
                          const unsigned int windowLength,
                          const unsigned int windowHeight)
{
    // Call super method for class
    NullRenderer::Init(openFullscreened, windowTitle, windowLength, windowHeight);

    // Make application instance. Vulkan programming guide (2016):
    // Vulkan includes a hierarchy of functionality, starting at the top level with the instance,
    // which aggregates all Vulkan-capable devices together. Each device then exposes one or more queues.
    CreateInstance(windowTitle);

    // Once we have an instance, we can use it to discover Vulkan-compatible devices installed in the system
    // The physical device usually represents a single piece of hardware or a collection of hardware
    // that is interconnected. There is a fixed, finite number of physical devices in any system
    // For this application, we will only use one device and choose the first compatible physical device we find
    return SelectPhysicalDevice();
}

VkSurfaceFormatKHR VulkanRenderer::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanRenderer::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

void VulkanRenderer::CreateSwapChain(int width, int height)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        surfaceFormats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.data());
    }

    std::vector<VkPresentModeKHR> presentModes;
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
    }

    surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);

    width = CLAMP(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
    height = CLAMP(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
    swapchainSize.width = width;
    swapchainSize.height = height;

    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = surfaceCapabilities.minImageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = swapchainSize;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {graphics_QueueFamilyIndex, present_QueueFamilyIndex};
    if (graphics_QueueFamilyIndex != present_QueueFamilyIndex)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain);

    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
    swapchainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());
}

void VulkanRenderer::SetupScreenAndCommand()
{
    CreateImageViews();
    SetupDepthStencil();
    CreateRenderPass();
    CreateFramebuffers();

    CreateCommandPool();
    CreateCommandBuffers();
    CreateSemaphores();
    CreateFences();
}
