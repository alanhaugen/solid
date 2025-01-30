#ifndef VULKANTEXTURE_H
#define VULKANTEXTURE_H

#include "modules/renderer/texture.h"

class VulkanTexture : public ITexture
{
public:
    VulkanTexture();
    VulkanTexture(String filename);
    VulkanTexture(String front, String back, String top, String bottom, String left, String right);

    ~VulkanTexture();

private:
};

#endif // VULKANTEXTURE_H
