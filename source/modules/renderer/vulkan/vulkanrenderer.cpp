#include "vulkanrenderer.h"
#include <set>
#include <fstream>

// VMA supports Vulkan version down to 1.0, for backward compatibility
// VMA compiles with code supporting the highest Vulkan version found in the included <vulkan/vulkan.h>
#define VMA_IMPLEMENTATION // This must be included in exactly one CPP file
#define VMA_VULKAN_VERSION 1000000 // Make VMA use Vulkan 1.0
#include "vk_mem_alloc.h"

// Thanks to https://gist.github.com/YukiSnowy/dc31f47448ac61dd6aedee18b5d53858 and https://vkguide.dev

// Sascha Willems has great resources for vulkan related things, such as for dynamic descriptor sets:
// https://github.com/SaschaWillems/Vulkan/blob/master/examples/dynamicuniformbuffer/README.md

#define CLAMP(x, lo, hi)    ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

void VulkanRenderer::ImmediateSubmit(std::function<void (VkCommandBuffer)> &&function)
{
    VkCommandBuffer cmd = uploadContext.commandBuffer;

    //begin the command buffer recording. We will use this command buffer exactly once before resetting, so we tell vulkan that
    VkCommandBufferBeginInfo cmdBeginInfo = {};//vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBeginInfo.pNext = nullptr;

    cmdBeginInfo.pInheritanceInfo = nullptr;
    cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(cmd, &cmdBeginInfo);

    //execute the function
    function(cmd);

    VkSubmitInfo info = {};// vkinit::submit_info(&cmd);

    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;

    info.waitSemaphoreCount = 0;
    info.pWaitSemaphores = nullptr;
    info.pWaitDstStageMask = nullptr;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &cmd;
    info.signalSemaphoreCount = 0;
    info.pSignalSemaphores = nullptr;

    //submit command buffer to the queue and execute it.
    // _uploadFence will now block until the graphic commands finish execution
    vkQueueSubmit(graphicsQueue, 1, &info, uploadContext.uploadFence);

    vkWaitForFences(device, 1, &uploadContext.uploadFence, true, 9999999999);
    vkResetFences(device, 1, &uploadContext.uploadFence);

    // reset the command buffers inside the command pool
    vkResetCommandPool(device, uploadContext.commandPool, 0);
}

VkCommandBufferBeginInfo VulkanRenderer::commandBufferBeginInfo(VkCommandBufferUsageFlags flags)
{
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.pNext = nullptr;

    info.pInheritanceInfo = nullptr;
    info.flags = flags;
    return info;
}

VkSubmitInfo VulkanRenderer::submitInfo(VkCommandBuffer *cmd)
{
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;

    info.waitSemaphoreCount = 0;
    info.pWaitSemaphores = nullptr;
    info.pWaitDstStageMask = nullptr;
    info.commandBufferCount = 1;
    info.pCommandBuffers = cmd;
    info.signalSemaphoreCount = 0;
    info.pSignalSemaphores = nullptr;

    return info;
}

void VulkanRenderer::SetupUploadContext()
{
    VkFenceCreateInfo uploadFenceCreateInfo;// = vkinit::fence_create_info();
    uploadFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    //uploadFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(device, &uploadFenceCreateInfo, nullptr, &uploadContext.uploadFence);

    VkCommandPoolCreateInfo uploadCommandPoolInfo = {};
    uploadCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    uploadCommandPoolInfo.queueFamilyIndex = graphics_QueueFamilyIndex;
    uploadCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(device, &uploadCommandPoolInfo, nullptr, &uploadContext.commandPool);

    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = uploadContext.commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(device, &allocateInfo, &uploadContext.commandBuffer);
}

void VulkanRenderer::CreateInstance(const char *windowTitle)
{
    // The root of everything is the VkInstance. In general, applications only need to
    // create a single VkInstance for their entire run, as it’s just the global Vulkan context for the application
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
    if (GetSupportedDepthFormat(physicalDevice, &depthFormat) == false)
    {
        LogError("Failed to find legal depth format");
    }

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

bool VulkanRenderer::LoadShader(const char* filePath, VkShaderModule* outShaderModule)
{
    // Open the file. With cursor at the end
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        return false;
    }


    // Find what the size of the file is by looking up the location of the cursor
    // because the cursor is at the end, it gives the size directly in bytes
    size_t fileSize = (size_t)file.tellg();

    // SPIRV expects the buffer to be on uint32, so make sure to reserve an int vector big enough for the entire file
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    // Put file cursor at beginning
    file.seekg(0);

    // Load the entire file into the buffer
    file.read((char*)buffer.data(), fileSize);

    // Now that the file is loaded into the buffer, we can close it
    file.close();

    // Create a new shader module, using the buffer we loaded
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;

    // CodeSize has to be in bytes,
    // so multiply the ints in the buffer by size of int to know the real size of the buffer
    createInfo.codeSize = buffer.size() * sizeof(uint32_t);
    createInfo.pCode = buffer.data();

    // Check that the creation goes well.
    VkShaderModule shaderModule;

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        return false;
    }

    *outShaderModule = shaderModule;

    return true;
}

VkPipelineShaderStageCreateInfo VulkanRenderer::ShaderPipelineStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
    VkPipelineShaderStageCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;

    //shader stage
    info.stage = stage;
    //module containing the code for this shader stage
    info.module = shaderModule;
    //the entry point of the shader
    info.pName = "main";
    return info;
}

VkPipeline VulkanRenderer::CreateGraphicsPipeline(VkDevice device, VkRenderPass pass,
                                                  const char* fragShaderPath, const char* vertShaderPath,
                                                  VulkanDrawable* drawable,
                                                  int topology)
{
    bool success = LoadShader(URL(vertShaderPath), &triangleVertShader);

    if (success == false)
    {
        LogError("Failed to load vertex shader: " + String(vertShaderPath));
        return VK_NULL_HANDLE;
    }

    success = LoadShader(URL(fragShaderPath), &triangleFragShader);

    if (success == false)
    {
        LogError("Failed to load fragment shader: " + String(fragShaderPath));
        return VK_NULL_HANDLE;
    }

    shaderStages.push_back(ShaderPipelineStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertShader));
    shaderStages.push_back(ShaderPipelineStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

    multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.pNext = nullptr;

    multisampling.sampleShadingEnable = VK_FALSE;
    //multisampling defaulted to no multisampling (1 sample per pixel)
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;

    // Setup vertex description (this is connected with the vertex attributes in the shader)
    VulkanDrawable::VertexInputDescription vertexDescription = drawable->GetVertexDescription();

    // Connect the pipeline builder vertex input info to the one we get from Vertex
    vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
    vertexInputInfo.vertexAttributeDescriptionCount = vertexDescription.attributes.size();

    vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
    vertexInputInfo.vertexBindingDescriptionCount = vertexDescription.bindings.size();

    inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.pNext = nullptr;

    /*     VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : normal triangle drawing
     *     VK_PRIMITIVE_TOPOLOGY_POINT_LIST : points
     *     VK_PRIMITIVE_TOPOLOGY_LINE_LIST : line-list
     **/

    if (topology == DRAW_TRIANGLES)
    {
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    else if (topology == DRAW_LINES)
    {
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }
    else if (topology == CUBEMAP)
    {
        LogWarning("Cubemaps not supported yet...");
    }
    else
    {
        LogError("Unsupported topology");
    }

    //we are not going to use primitive restart on the entire tutorial so leave it on false
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.pNext = nullptr;

    rasterizer.depthClampEnable = VK_FALSE;
    //discards all primitives before the rasterization stage if enabled which we don't want
    rasterizer.rasterizerDiscardEnable = VK_FALSE;

    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    //no backface cull
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    //no depth bias
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // Make viewport state from our stored viewport and scissor.
    // At the moment we won't support multiple viewports or scissors
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapchainSize.width;
    viewport.height = (float)swapchainSize.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissor.offset = { 0, 0 };
    scissor.extent = swapchainSize;

    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Controls how this pipeline blends into a given attachment. We are rendering to only 1 attachment,
    // so we will just need one of them, and defaulted to “not blend” and just override. In here it’s possible
    // to make objects that will blend with the image. This one also doesn’t have sType + pNext
    colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    // Setup dummy color blending. We aren't using transparent objects yet
    // The blending is just "no blend", but we do write to the color attachment
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Setup descriptor layout which is the uniform buffer block for the shader
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = 2;
    VkDescriptorSetLayout layouts[2] = { uniformSetLayout, textureSetLayout };
    pipelineLayoutInfo.pSetLayouts = layouts;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &drawable->pipelineLayout);

    // Depth test
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.pNext = nullptr;

    depthStencilInfo.depthTestEnable = VK_TRUE;
    depthStencilInfo.depthWriteEnable = VK_TRUE;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.minDepthBounds = 0.0f; // Optional
    depthStencilInfo.maxDepthBounds = 1.0f; // Optional
    depthStencilInfo.stencilTestEnable = VK_FALSE;

    // Build the actual pipeline
    // We now use all of the info structs we have been writing into into this one to create the pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = drawable->pipelineLayout;
    pipelineInfo.pDepthStencilState = &depthStencilInfo;
    pipelineInfo.renderPass = pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    // It's easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
    VkPipeline newPipeline;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
    {
        LogError("Failed to create shader pipeline");
        return VK_NULL_HANDLE; // failed to create graphics pipeline
    }
    else
    {
        return newPipeline;
    }
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

void VulkanRenderer::SetupVMA()
{
    // Initialize the AMD Vulkan Memory Allocator
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = physicalDevice;
    allocatorInfo.device = device;
    allocatorInfo.instance = instance;
    vmaCreateAllocator(&allocatorInfo, &allocator);
}

void VulkanRenderer::SetupDescriptionPool()
{
    // Create a descriptor pool that will hold the dynamic uniform buffer and image sampler+textures
    std::vector<VkDescriptorPoolSize> sizes =
    {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2048 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;
    pool_info.maxSets = 2049;
    pool_info.poolSizeCount = (uint32_t)sizes.size();
    pool_info.pPoolSizes = sizes.data();

    vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptorPool);
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
    // Once we have the VkPhysicalDevice of the GPU we are going to use, we can create a VkDevice from it
    // Let us request the extension VK_KHR_SWAPCHAIN for backbuffer support
    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
//        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
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

    vkDestroyShaderModule(device, triangleVertShader, nullptr);
    vkDestroyShaderModule(device, triangleFragShader, nullptr);

    vkDestroyDescriptorSetLayout(device, uniformSetLayout, nullptr);
    //vkDestroyDescriptorSetLayout(device, singleTextureSetLayout, nullptr);
    vmaDestroyBuffer(allocator, uniformBuffer.buffer, uniformBuffer.allocation);

    // Clean up the drawables
    LinkedList<VulkanDrawable*>::Iterator drawable = drawables.Begin();

    for (; drawable != NULL; ++drawable)
    {
        delete (*drawable);
    }

    // Destroy vma
    //vmaDestroyAllocator(allocator); // FIXME: I get a segmentation error for some reason

    // Clean up desciptor pool
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    // Delete command pool
    vkDestroyCommandPool(device, commandPool, nullptr);

    // Destroy fences
    for (uint32_t i = 0; i < swapchainImageCount; i++)
    {
        vkDestroyFence(device, fences[i], nullptr);
    }

    vkDestroyFence(device, uploadContext.uploadFence, nullptr);;
    vkDestroyCommandPool(device, uploadContext.commandPool, nullptr);

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

    VkClearColorValue clear_color = { {1.0f, 0.0f, 0.0f, 1.0f} };
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

    VkClearValue depthClear;
    depthClear.depthStencil.depth = 1.f;

    std::vector<VkClearValue> clearValues(2);
    clearValues[0].color = clear_color;
    clearValues[1].depthStencil = clear_depth_stencil;

    render_pass_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
    render_pass_info.pClearValues = clearValues.data();

    // In Vulkan, all of the rendering happens inside a VkRenderPass (NOTE: For Vulkan 1.3, this has changed)
    vkCmdBeginRenderPass(commandBuffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    // Rendering commands go here
    LinkedList<VulkanDrawable*>::Iterator drawable = drawables.Begin();

    for (; drawable != NULL; ++drawable)
    {
        if ((*drawable)->draw == false || (*drawable)->verticesQuantity == 0)
        {
            continue;
        }

        (*drawable)->draw = false;

        // Upload uniforms to shader program
        (*drawable)->UploadUniformBufferBlock(projViewMatrixArray[0]);

        // TODO: Only bind pipeline and bind descriptor sets when the pipeline actually needs to change (per shader)
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (*drawable)->pipeline);

        // Bind descriptor set (shader uniforms)
        uint32_t uniformOffset = PadUniformBufferSize(sizeof(UniformBlock) * (*drawable)->offset);

        // Descriptor set 0 has the uniforms
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (*drawable)->pipelineLayout, 0, 1, &uniformDescriptor, 1, &uniformOffset);

        // Descriptor set 1 has the texture and sampler
        if ((*drawable)->isTextured)
        {
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (*drawable)->pipelineLayout, 1, 1, &(*drawable)->textureDescriptor, 0, nullptr);
        }

        // Bind the mesh vertex buffer with offset 0
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &(*drawable)->vertexBuffer.buffer, &offset);

        if ((*drawable)->indicesQuantity != 0)
        {
            // Bind index buffer
            vkCmdBindIndexBuffer(commandBuffer, (*drawable)->indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdDrawIndexed(commandBuffer, (*drawable)->indicesQuantity, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, (*drawable)->verticesQuantity, 1, 0, 0);
        }
    }

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

size_t VulkanRenderer::PadUniformBufferSize(size_t originalSize)
{
    // Calculate required alignment based on minimum device offset alignment
    size_t minUboAlignment = gpuProperties.limits.minUniformBufferOffsetAlignment;
    size_t alignedSize = originalSize;
    if (minUboAlignment > 0) {
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
}

// This was for descritor indexing, which I have abandoned
void VulkanRenderer::UploadTexturesToGPU()
{
    /*for (unsigned i = texturesUploaded; i < textures.Size(); i++) //NumDescriptorsNonUniform; i++)
    {
        VkDescriptorImageInfo image_info = {};
        image_info.sampler               = VK_NULL_HANDLE;
        image_info.imageView             = textures[i]->imageView;
        image_info.sampler               = blockySampler;
        image_info.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet write = {};
        write.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet               = textureDescriptor;
        write.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.dstBinding           = 0;
        write.descriptorCount      = 1;
        write.pImageInfo           = &image_info;

        write.dstArrayElement = i;

        vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);

        texturesUploaded++;
    }*/
}

ITexture *VulkanRenderer::CreateTexture(String filename)
{
    VulkanTexture *texture = NULL;//; FindTexture(filename);

    if (texture == NULL)
    {
        texture = new VulkanTexture(filename, device, physicalDevice, allocator);

        textures.Add(texture);
    }

    //UploadTexturesToGPU();

    return texture;
}

ITexture *VulkanRenderer::CreateTexture(String front, String back, String top, String bottom, String left, String right)
{
    VulkanTexture *texture = NULL;//; FindTexture(filename);

    if (texture == NULL)
    {
        texture = new VulkanTexture(front, back, top, bottom, left, right);

        textures.Add(texture);
    }

    //UploadTexturesToGPU();

    return texture;
}

ITexture *VulkanRenderer::CreateTexture(int width, int height)
{
    return NULL;
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
    // that is interconnected. There is a fixed, finite number of physical devices in any system.
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
    // Initializing the GPU is nice, but we want to actually perform some rendering into the screen.
    // We use a swapchain for that. A swapchain is a OS/windowing provided structure with some images
    // we can draw to and then display on the screen. Swapchains are not in the core Vulkan spec as
    // they are optional, and often unique to the different platforms. If you are going to use Vulkan
    // for compute shader calculations, or for offline rendering, you do not need to setup a swapchain
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

bool VulkanRenderer::SetupScreenAndCommand()
{
    CreateImageViews();
    SetupDepthStencil();
    CreateRenderPass();
    CreateFramebuffers();

    CreateCommandPool();
    CreateCommandBuffers();
    CreateSemaphores();
    CreateFences();

    vkGetPhysicalDeviceProperties(physicalDevice, &gpuProperties);

    SetupVMA();
    SetupDescriptionPool();

    SetupUploadContext();

    SetupDescriptorSets();

    return true;
}

void VulkanRenderer::SetupDescriptorSets()
{
    // Information about the binding.
    VkDescriptorSetLayoutBinding bufferBinding = {};
    bufferBinding.binding = 0;
    bufferBinding.descriptorCount = 1;
    // This is a dynamic uniform buffer binding
    // We use dynamic because we will have per-object data, this is very popular in game engines
    // 8 dynamic uniform buffers is required by the Vulkan specification, we will only use one
    bufferBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    bufferBinding.pImmutableSamplers = nullptr;

    // we use it from the vertex shader
    bufferBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    // We will try to have descriptor indexing
    // This is because I can't figure out textures in Vulkan
    // I think the descritor indexing extension makes more sense
    // than the other ways of doing textures (one descriptor per texture??)
    VkDescriptorSetLayoutCreateInfo setinfo = {};
    setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setinfo.flags = 0;

    // Set the binding flags above (for descriptor indexing: VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT_EXT)
    setinfo.pNext = nullptr;

    // We are going to have 1 binding
    setinfo.bindingCount = 1;
    // Point to the camera buffer binding
    setinfo.pBindings = &bufferBinding;

    // Setup the only descriptor set to be used for our applications
    vkCreateDescriptorSetLayout(device, &setinfo, nullptr, &uniformSetLayout);

    // Allocate descriptor set
    VkDescriptorSetAllocateInfo allocInfo ={};
    allocInfo.pNext = nullptr;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    //using the pool we just set
    allocInfo.descriptorPool = descriptorPool;
    //only 1 descriptor
    allocInfo.descriptorSetCount = 1;
    //using the uniform data layout
    allocInfo.pSetLayouts = &uniformSetLayout;

    vkAllocateDescriptorSets(device, &allocInfo, &uniformDescriptor);

    // Allocate buffer block data
    uniformBuffer = CreateBuffer(PadUniformBufferSize(sizeof(UniformBlock) * 2048),
                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                 VMA_MEMORY_USAGE_CPU_TO_GPU);

    // Point descriptor set to uniform buffer
    VkDescriptorBufferInfo binfo;
    //it will be the uniform buffer
    binfo.buffer = uniformBuffer.buffer;
    //at 0 offset
    binfo.offset = 0;
    //of the size of a camera data struct
    binfo.range = sizeof(UniformBlock);

    VkWriteDescriptorSet setWrite = {};
    setWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    setWrite.pNext = nullptr;

    //we are going to write into binding number 0
    setWrite.dstBinding = 0;
    //of the global descriptor
    setWrite.dstSet = uniformDescriptor;

    setWrite.descriptorCount = 1;
    //and the type is uniform buffer
    setWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    setWrite.pBufferInfo = &binfo;

    vkUpdateDescriptorSets(device, 1, &setWrite, 0, nullptr);

    VkDescriptorSetLayoutBinding textureBinding = {};
    textureBinding.binding = 0;
    textureBinding.descriptorCount = 1; // number of textures
    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    textureBinding.pImmutableSamplers = nullptr;

    // Texture binding
    setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
    setinfo.pBindings = &textureBinding;

    vkCreateDescriptorSetLayout(device, &setinfo, nullptr, &textureSetLayout);
}

AllocatedBuffer VulkanRenderer::CreateBuffer(size_t allocSize,
                                             VkBufferUsageFlags usage,
                                             VmaMemoryUsage memoryUsage)
{
    // Allocate buffer
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;

    bufferInfo.size = allocSize;
    bufferInfo.usage = usage;

    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = memoryUsage;

    AllocatedBuffer newBuffer;

    // Allocate the buffer
    vmaCreateBuffer(allocator, &bufferInfo, &vmaallocInfo,
                    &newBuffer.buffer,
                    &newBuffer.allocation,
                    nullptr);

    return newBuffer;
}

IDrawable *VulkanRenderer::CreateDrawable(Array<IDrawable::Vertex> &vertices,
                                          Array<unsigned int> &indices,
                                          Array<String> &shaders,
                                          Array<ITexture *> textures,
                                          int topology)
{
    VulkanShader* shader = new VulkanShader();

    VulkanDrawable *drawable = new VulkanDrawable(vertices,
                                                  indices,
                                                  shader,
                                                  textures,
                                                  allocator,
                                                  device,
                                                  descriptorPool,
                                                  textureSetLayout,
                                                  uniformBuffer,
                                                  drawables.Size() + 1);

    drawable->pipeline = CreateGraphicsPipeline(device,
                                                render_pass,
                                                shaders[FRAGMENT_SHADER], shaders[VERTEX_SHADER],
                                                drawable,
                                                topology);

    drawables.Append(drawable);

    return drawable;
}

IDrawable *VulkanRenderer::CreateDrawable(Array<IDrawable::Vertex> &vertices,
                                          Array<unsigned int> &indices,
                                          Array<String> &shaders,
                                          ITexture *texture,
                                          int topology)
{
    Array<ITexture *> textures;

    if (texture != NULL)
    {
        textures.Add(texture);
    }

    VulkanShader* shader = new VulkanShader();

    VulkanDrawable* drawable = new VulkanDrawable(vertices,
                                                  indices,
                                                  shader,
                                                  textures,
                                                  allocator,
                                                  device,
                                                  descriptorPool,
                                                  textureSetLayout,
                                                  uniformBuffer,
                                                  drawables.Size() + 1);

    drawable->pipeline = CreateGraphicsPipeline(device, render_pass,
                                                shaders[FRAGMENT_SHADER], shaders[VERTEX_SHADER],
                                                drawable,
                                                topology);

    drawables.Append(drawable);

    return drawable;
}

void VulkanRenderer::RemoveDrawable(IDrawable *drawable)
{
    LinkedList<VulkanDrawable*>::Iterator drawableIterator = drawables.Begin();

    for (int i = 0; drawableIterator != drawables.End(); ++drawableIterator)
    {
        VulkanDrawable *vulkanDrawable = (*drawableIterator);
        if (vulkanDrawable == drawable)
        {
            drawables.RemoveAt(i);
            delete vulkanDrawable;
            break;
        }

        i++;
    }
}
