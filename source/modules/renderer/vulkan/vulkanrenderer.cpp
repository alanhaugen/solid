#include "vulkanrenderer.h"
#include "vulkandrawable.h"
#include <set>

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

    vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);

    return true;
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
{
   return true;
}

VulkanRenderer::~VulkanRenderer()
{
    // Cleanly shut down the Vulkan system by waiting for a device to complete work submitted by
    // the application, destroying the device handles, and finally destroying the instance hand
    vkDeviceWaitIdle(device);

    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
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

void VulkanRenderer::CreateSwapChain(int width, int height)
{
    //CreateSwapChain
}
