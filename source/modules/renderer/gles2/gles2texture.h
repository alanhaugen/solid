#ifndef GLES2TEXTURE_H
#define GLES2TEXTURE_H

#include <glad/gl.h>
#include "core/x-platform/locator.h"

const int STANDARD = 0;

class GLES2Texture
{
public:
    GLES2Texture();
    GLES2Texture(const char *filePath);
    void Load(class Pixmap* pixmap, int type = STANDARD, GLenum sideTarget = 0);

    void Activate();

    ~GLES2Texture();

    GLuint textureID;
private:
    void Load();

    int width, height, bitDepth;
    const char *filePath;
};

#endif // GLES2TEXTURE_H
