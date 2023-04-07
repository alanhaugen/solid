#ifndef CAFE_FILE_H
#define CAFE_FILE_H

#include <cafe.h>
#include <cafe/fs.h>
#include <cstddef>
#include "core/x-platform/url.h"
#include "modules/filesystem/file.h"

class CafeFile : public IFile
{
public:
    CafeFile();
    ~CafeFile();
    CafeFile(FSClient *client_, FSCmdBlock *cmd_, URL filePath, const unsigned int fileType_);

    char *Read();

    bool Write(char *data, unsigned int size);

    int ReadInteger();
    float ReadFloat();
    char *ReadString(int chars);

    long Size();

private:
    void Init();

    unsigned int fileType;
    FSFileHandle fh;
    FSClient   *client;
    FSCmdBlock *cmd;

    char *buffer;
    FSSize filesize;
};

#endif // FILE_H
