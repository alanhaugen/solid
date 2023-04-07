#include "gles2texture.h"
//#define STBI_ONLY_PNG
//#define STBI_NO_STDIO
#include "3rdparty/stb_image.h"

#include "core/x-platform/pixmap.h"

// TODO: implement multi texture support

GLES2Texture::GLES2Texture()
    : textureID(0u)
    , width(0)
    , height(0)
    , bitDepth(0)
    , filePath("")
{
    glGenTextures(1, &textureID);
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

void GLES2Texture::Load(class Pixmap* pixmap, int type, GLenum sideTarget)
{
    assert(pixmap);

    if (type == STANDARD)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum internal_format = GL_RGBA;
        GLenum format = pixmap->channels == Pixmap::RGB ? GL_RGB : GL_RGBA;

        glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    internal_format,
                    pixmap->width,
                    pixmap->height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    pixmap->data);

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

        GLenum format = pixmap->channels == Pixmap::RGB ? GL_RGB : GL_RGBA;

        // copy image data into 'target' side of cube map
        glTexImage2D(
                    sideTarget,
                    0,
                    GL_RGBA,
                    pixmap->width,
                    pixmap->height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    pixmap->data);
    }

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
