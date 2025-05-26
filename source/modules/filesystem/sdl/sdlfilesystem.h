#ifndef STDFILESYSTEM_H
#define STDFILESYSTEM_H

#include "modules/filesystem/filesystem.h"
#include "core/x-platform/string.h"
#include "sdlfile.h"

class StdFileSystem : public NullFileSystem
{
public:
	StdFileSystem();

	bool Shutdown();

    StdFile *Open(String path, const unsigned int dataType = DATA);
};

#endif  // STDFILESYSTEM_H
