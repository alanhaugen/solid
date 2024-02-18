#ifndef GLES2TEXTURE_H
#define GLES2TEXTURE_H

#include <glad/gl.h>
#include "modules/renderer/texture.h"
#include "core/x-platform/locator.h"
#include "core/x-platform/typedefs.h"

const int STANDARD = 0;

class GLES2Texture : public ITexture
{
public:
    GLES2Texture();
    GLES2Texture(String filename);
    GLES2Texture(String front, String back, String top, String bottom, String left, String right);

    virtual U8 At(unsigned int x, unsigned int y, U8 data);
    virtual U8 At(unsigned int index, U8 data);

    virtual void ReUpload();
    virtual void ReUpload(String filePath);

    void Load();
    void Load(String path, int type = STANDARD, GLenum sideTarget = 0);

    void Activate();

    ~GLES2Texture();

    GLuint textureID;

private:

    int bitDepth;
};

#endif // GLES2TEXTURE_H
