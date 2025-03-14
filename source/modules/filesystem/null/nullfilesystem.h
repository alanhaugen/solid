#ifndef NULL_FILESYSTEM_H
#define NULL_FILESYSTEM_H

#include "modules/filesystem/filesystem.h"
#include "nullfile.h"
#include "core/x-platform/url.h"

class NullFileSystem : public IFileSystem {
public:
  NullFileSystem();

  virtual bool Init();

  virtual bool Shutdown();

  virtual NullFile *Open(String path, const unsigned int fileType = DATA);
};

#endif
