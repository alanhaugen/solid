#ifndef STDFILE_H
#define STDFILE_H

#include <stdio.h>
#include "core/x-platform/url.h"
#include "core/x-platform/locator.h"
#include "modules/filesystem/null/nullfile.h"

class StdFile : public NullFile
{
private:
    FILE *file;
    char *buffer;
    long filesize;

public:
    StdFile(URL filePath, const unsigned int fileType);
    ~StdFile();

    char *Read();

    bool Write(char *data, unsigned int size);

    long Size();

    int ReadInteger();
    float ReadFloat();
    char *ReadString(int chars);
};

#endif // STDFILE_H
