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

    virtual U8 At(unsigned int x, unsigned int y, U8 data);
    virtual U8 At(unsigned int index, U8 data);

    virtual void ReUpload();
    virtual void ReUpload(String filePath);

    void Load(int type = STANDARD, GLenum sideTarget = 0);

    void Activate();

    ~GLES2Texture();

    GLuint textureID;

private:
    void Load();

    int bitDepth;
};

#endif // GLES2TEXTURE_H
