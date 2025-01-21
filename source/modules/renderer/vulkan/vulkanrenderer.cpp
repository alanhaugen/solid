#include "vulkanrenderer.h"
#include "vulkandrawable.h"

bool VulkanRenderer::Init(bool openFullscreened, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight)
{
    // Make application instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = windowTitle;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Solid";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledLayerCount = validationLayers.size();
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    instanceCreateInfo.enabledExtensionCount = extensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

    vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

    // Make swapchain
    //vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_devices, surface, &surfaceCapabilities);

    //std::vector<VkSurfaceFormatKHR> surfaceFormats;

    //uint32_t formatCount;
    //vkGetPhysicalDeviceSurfaceFormatsKHR(physical_devices, surface, &formatCount, nullptr);

    return true;
}
