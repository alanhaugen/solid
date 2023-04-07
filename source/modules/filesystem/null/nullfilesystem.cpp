#include "nullfilesystem.h"

NullFileSystem::NullFileSystem() {
}

bool NullFileSystem::Init() {
    return true;
}

bool NullFileSystem::Shutdown() {
    return true;
}

NullFile *NullFileSystem::Open(URL path, const unsigned int fileType) {
    (void)path;
    NullFile *nullfile = new NullFile(fileType);
    return nullfile;
}
