#include "core/x-platform/typedefs.h"
#include "core/x-platform/string.h"

enum TEXTURE_CHANNELS
{
    RGB = 3,
    RGBA = 4
};

class ITexture
{
public:
    String name;
    bool isLoaded;
    bool isManipulated;

    virtual U8 At(unsigned int x, unsigned int y, U8 data) = 0;
    virtual U8 At(unsigned int index, U8 data) = 0;

    virtual void ReUpload() = 0;
    virtual void ReUpload(String pathName) = 0;

private:
    unsigned int width;
    unsigned int heigt;
    int channels;

    U8* data;
};

