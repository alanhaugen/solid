#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "file.h"
#include "core/x-platform/url.h"

class IFileSystem {
public:
    virtual bool Init() = 0;
    virtual bool Shutdown() = 0;
    virtual IFile *Open(URL url, const unsigned int fileType = DATA) = 0;

    virtual ~IFileSystem() {}
};

#endif  // FILESYSTEM_H
