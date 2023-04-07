#include "pixmap.h"
#include "core/x-platform/locator.h" // for Log
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stb_image.h"

#include <stdio.h>

Pixmap::~Pixmap()
{
    delete[] data;
}

Pixmap::Pixmap()
    : filePath("")
{
    Pixmap(8, 8);
}

Pixmap::Pixmap(int width_, int height_)
    : filePath("")
{
    width = width_;
    height = height_;

    channels = RGB;
    data = new U8[width*height*channels];

    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            int idx = channels * (j*width + i);
            for(int c=0; c < channels; c++) {
                data[idx+c] = 255;
            }
        }
    }

    //Log("you made a pixmap by default:" + filePath + ", h:"+String(width) + ", h:"+String(height) + ", ch:" + String(channels) );
}

Pixmap::Pixmap(const String& name, U32 _width, U32 _height, eCHANNELS channels)
    : filePath(URL(name).ToChar())
    , width(_width)
    , height(_height)
{
    assert( RGB == 3 );
    assert( RGBA == 4 );
    data = new U8[width*height*channels];
    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            int idx = channels * (j*width + i);
            bool isOdd = (i + j) % 2 == 0;
            data[idx+0] = 255 * !isOdd;
            data[idx+1] = 255 * isOdd;
            data[idx+2] = 0;
        }
    }

    //Log("you made a pixmap by name:" + filePath + ", h:"+String(width) + ", h:"+String(height) + ", ch:" + String(channels) );
}

Pixmap::Pixmap(const String &_filePath) : filePath(URL(_filePath).ToChar())
{
    int n;

    IFile *file = Locator::filesystem->Open(URL(_filePath));

    unsigned char *img = stbi_load_from_memory((const unsigned char*)file->Read(), file->Size(), &width, &height, &n, 0);

    assert (img);

    delete file;

    if (n==3) channels = RGB;
    else if (n==4) channels = RGBA;
    else LogError("Image not suported. RGB and RGBA grayscale is supported");
    data = new U8[width*height*channels];
    // A bit stupid to copy data when we can just use img pointer and free it ourselves on dtor
    for(int j=0; j<height; j++)
    {
        for(int i=0; i<width; i++)
        {
            int idx = channels * (j*width + i);
            for(int c=0; c<n; c++) {
                data[idx+c] = img[idx+c];
            }
        }
    }
    stbi_image_free(img);

    //Log("Loaded pixmap/image from file:" + filePath + ", h:"+String(width) + ", h:"+String(height) + ", ch:" + String(channels) );
}

Array<Pixmap*> Pixmap::pixmapCache; // Declare our static variable in file scope
/*static */Pixmap* Pixmap::LoadAndCache(const String& filePath)
{
    // Check if we added it previously
    for(U32 i=0; i<pixmapCache.Size(); i++)
    {
        if ( pixmapCache[i]->filePath == filePath )
        {
            return pixmapCache[i];
        }
    }

    // We must load it from disk
    Pixmap* ret = new Pixmap(filePath);
    pixmapCache.Add(ret);
    return ret;
}

void Pixmap::ClearCache()
{
    for(U32 i=0; i<pixmapCache.Size(); i++)
    {
        delete pixmapCache[i];
    }
    pixmapCache.Clear();
}
