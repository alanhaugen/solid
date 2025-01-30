#ifndef VULKANTEXTURE_H
#define VULKANTEXTURE_H

#include "modules/renderer/texture.h"

class VulkanTexture : public ITexture
{
public:
    VulkanTexture();
    VulkanTexture(String filename);
    VulkanTexture(String front, String back, String top, String bottom, String left, String right);

    U8 At(unsigned int x, unsigned int y, U8 data);
    U8 At(unsigned int index, U8 data);

    void ReUpload();
    void ReUpload(String filePath);

    ~VulkanTexture();

private:
};

#endif // VULKANTEXTURE_H
