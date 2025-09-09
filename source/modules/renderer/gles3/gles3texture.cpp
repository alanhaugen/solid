#include "gles3texture.h"
#include "core/x-platform/locator.h"

//#define STBI_ONLY_PNG
//#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

GLES3Texture::GLES3Texture()
    : textureID(0u)
    , bitDepth(0)
{
    glGenTextures(1, &textureID); // number of texture names, texture name array
}

GLES3Texture::GLES3Texture(String filePath)
    :
      textureID(0u),
      bitDepth(0)
{
    name = filePath;

    glGenTextures(1, &textureID);
    Load();
}

GLES3Texture::GLES3Texture(String front, String back, String top, String bottom, String left, String right)
{
    name = front;

    Load(front, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    Load(back, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    Load(top, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    Load(bottom, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    Load(left, CUBEMAP, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    Load(right, CUBEMAP, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}

U8 GLES3Texture::At(unsigned int x, unsigned int y, U8 data)
{
    return 0;
}

U8 GLES3Texture::At(unsigned int index, U8 data)
{
    return 0;
}

void GLES3Texture::ReUpload()
{

}

void GLES3Texture::ReUpload(String filePath)
{
    name = filePath;
}

void GLES3Texture::Load(String path, int type, GLenum sideTarget)
{
    IFile *file;

    if (path == "")
    {
        file = Locator::filesystem->Open(name);
    }
    else
    {
        file = Locator::filesystem->Open(path);
    }

    unsigned char *img = stbi_load_from_memory((const unsigned char*)file->Read(), file->Size(), &width, &height, &channels, 0);

    GLenum internal_format = GL_RGBA;
    GLenum format;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (channels == TEXTURE_CHANNELS::RGB)
    {
        format = GL_RGB;
    }
    else
    {
        format = GL_RGBA;
    }

    if (type == STANDARD)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, textureID);


        glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    internal_format,
                    width,
                    height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    img);

        //glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (type == CUBEMAP)
    {
        // Thanks to https://antongerdelan.net/opengl/cubemaps.html
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // copy image data into 'target' side of cube map
        glTexImage2D(
                    sideTarget,
                    0,
                    internal_format,
                    width,
                    height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    img);
    }

    delete file;
    stbi_image_free(img);
}

void GLES3Texture::Activate()
{
    // TODO: Add multitexture support (GL_TEXTURE1 etc?)
    if (textureID != 0u)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

GLES3Texture::~GLES3Texture()
{
    glDeleteTextures(1, &textureID);
}
