#ifndef CAFE_FILESYSTEM_H
#define CAFE_FILESYSTEM_H

#include <cafe.h>
#include <cafe/fs.h>
#include "modules/filesystem/filesystem.h"
#include "cafefile.h"
#include "core/x-platform/url.h"

class CafeFileSystem : public IFileSystem
{
public:
    CafeFileSystem();
    ~CafeFileSystem();

    virtual bool Init();

    virtual bool Shutdown();

    virtual CafeFile *Open(URL path, const unsigned int fileType = DATA);

private:
    FSClient   *client;
    FSCmdBlock *cmd;
};

#endif
