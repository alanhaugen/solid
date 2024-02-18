#include "gles2texture.h"

#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

GLES2Texture::GLES2Texture()
    : textureID(0u)
    , width(0)
    , height(0)
    , bitDepth(0)
    , filePath("")
{
    glGenTextures(1, &textureID); // number of texture names, texture name array
}

GLES2Texture::GLES2Texture(const char *filePath)
    :
      textureID(0u),
      width(0),
      height(0),
      bitDepth(0),
      filePath(filePath)
{
    glGenTextures(1, &textureID);
    Load();
}

void GLES2Texture::Load(int type, GLenum sideTarget)
{
    IFile *file = filesystem->Open(filePath);
    unsigned char *img = stbi_load_from_memory((const unsigned char*)file->Read(), file->Size(), &width, &height, &channels, 0);

    GLenum internal_format = GL_RGBA;
    GLenum format;

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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

        glGenerateMipmap(GL_TEXTURE_2D);

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

void GLES2Texture::Load()
{
    unsigned char *texData = stbi_load(filePath, &width, &height, &bitDepth, 0);

    if (!texData)
    {
        LogError("Failed to find: " + String(filePath));
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
}

void GLES2Texture::Activate()
{
    // TODO: Add multitexture support (GL_TEXTURE1 etc?)
    if (textureID != 0u)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}

GLES2Texture::~GLES2Texture()
{
    glDeleteTextures(1, &textureID);
}
