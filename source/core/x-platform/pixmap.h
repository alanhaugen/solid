#ifndef PIXMAP_H
#define PIXMAP_H

#include "core/x-platform/string.h"
#include "core/x-platform/typedefs.h"
#include "core/containers/array.h"

class Pixmap
{
public:
    enum eCHANNELS{ RGB=3, RGBA=4 };
    ~Pixmap();
    Pixmap();
    Pixmap(int width, int height);
    Pixmap(int r, int g, int b, int width = 8, int height = 8);
    Pixmap(const String& name, U32 _width, U32 _height, eCHANNELS channels);
    Pixmap(const String& _filePath);

    static Pixmap* LoadAndCache(const String& filePath);
    static void    ClearCache();

    String filePath;
    int width;
    int height;
    int channels;
    U8* data;

private:
    static Array<Pixmap*> pixmapCache;
};

#endif // PIXMAP_H
