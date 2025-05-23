#include "stdfile.h"
//#include "core/x-platform/typedefs.h"
//#include <sys/stat.h> // For filesize

#include <SDL_rwops.h>
#include <core/x-platform/string.h>

StdFile::StdFile(String filePath, const unsigned int fileType)
{
    String path = filePath;
    buffer = SDL_LoadFile(path.ToChar(), &filesize);

    if (buffer == NULL)
    {
        LogWarning(String("Failed to open file " + path + " (" + filePath + ")"));
    }
}

StdFile::~StdFile()
{
    if(buffer != NULL)
    {
        SDL_free(buffer);
	buffer = NULL;
    }
}

long StdFile::Size()
{
    return filesize;
}

char *StdFile::Read()
{
    return (char*)buffer;
}

int StdFile::ReadInteger()
{
    /*int i;
    fread(&i, sizeof(int), 1, file);

    isEOF = feof(file);

    return i;*/
}

float StdFile::ReadFloat()
{
    /*float f = 0.0f;
    fread(&f, sizeof(float), 1, file);

    isEOF = feof(file);

    return f;*/
}

char *StdFile::ReadString(int chars)
{
    return (char*)buffer;
}

bool StdFile::Write(char *data, unsigned int size)
{
    UNUSED(data);
    UNUSED(size);
    return false;
}
