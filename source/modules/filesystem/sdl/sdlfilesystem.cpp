#include "sdlfilesystem.h"

StdFileSystem::StdFileSystem()
{
}

bool StdFileSystem::Shutdown()
{
    // TODO: Close all open files
    // TODO: Log warning for each file...
    return true;
}

StdFile *StdFileSystem::Open(String path, const unsigned int dataType)
{
    StdFile *stdfile = new StdFile(path, dataType);
    return stdfile;
}
