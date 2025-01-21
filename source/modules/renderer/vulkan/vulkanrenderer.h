#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "modules/renderer/null/nullrenderer.h"

class VulkanRenderer : public Renderer::NullRenderer
{
protected:
public:
    VkInstance instance;
    std::vector<const char *> extensionNames;
    const std::vector<const char*> validationLayers;

    bool Init(bool openFullscreened,
              const char *windowTitle,
              const unsigned int windowLength,
              const unsigned int windowHeight);
};

#endif
