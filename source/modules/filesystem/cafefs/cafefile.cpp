#include "cafefile.h"

CafeFile::CafeFile()
{
    Init();
}

void CafeFile::Init()
{
    location = 0;
    isEOF = false;
    buffer = NULL;
    filesize = 0;
}

CafeFile::~CafeFile()
{
    FSCloseFile(client, cmd, fh, FS_RET_NO_ERROR);

    if (buffer != NULL)
    {
        MEMFreeToDefaultHeap(buffer);
        buffer = NULL;
    }
}

CafeFile::CafeFile(FSClient *client_, FSCmdBlock *cmd_, URL filePath, const unsigned int fileType_)
{
    Init();

    fileType = fileType_;
    client   = client_;
    cmd      = cmd_;

    String path = String("/vol/content/") + filePath.ToChar();

    FSOpenFile(client, cmd, path.ToChar(), "r", &fh, FS_RET_NO_ERROR);
}

bool CafeFile::Write(char *data, unsigned int size)
{
    (void)data;
    (void)size;
    return false;
}

char *CafeFile::Read()
{
    if (buffer != NULL)
    {
        return buffer;
    }

    FSStatus status;

    FSStat *stat;

    stat = (FSStat*) MEMAllocFromDefaultHeap(sizeof(FSStat));
    if (!stat)
    {
        OSHalt("Error: cannot allocate memory for fs client.\n");
    }

    status = FSGetStatFile(client, cmd, fh, stat, FS_RET_NO_ERROR);

    if (status != FS_STATUS_OK)
    {
        OSHalt("Error: failed to get file status information.\n");
    }

    filesize = stat->size;

    if (fileType == PLAIN_TEXT)
    {
        buffer = (char*) MEMAllocFromDefaultHeapEx(filesize + 1, FS_IO_BUFFER_ALIGN);
    }
    else
    {
        buffer = (char*) MEMAllocFromDefaultHeapEx(filesize, FS_IO_BUFFER_ALIGN);
    }

    if (!buffer)
    {
        OSHalt("Error: cannot allocate memory for fs client.\n");
    }

    FSStatus bytesRead = FSReadFile(client, cmd, buffer, 1, filesize, fh, 0, FS_RET_NO_ERROR);

    if (bytesRead != stat->size)
    {
        OSHalt("Error: failed to read entire file into buffer.\n");
    }

    if (fileType == PLAIN_TEXT) buffer[filesize] = 0;

    MEMFreeToDefaultHeap(stat);

    return buffer;
}

int CafeFile::ReadInteger()
{
    int i = 0;

    Read();

    memcpy(&i, buffer + location, sizeof(int));

    location += sizeof(int);

    if (location == filesize) isEOF = true;

    return i;
}

float CafeFile::ReadFloat()
{
    float f = 0.0f;

    Read();

    memcpy(&f, buffer + location, sizeof(float));

    location += sizeof(float);

    if (location == filesize) isEOF = true;

    return f;
}

char *CafeFile::ReadString(int chars)
{
    Read();

    char *string = new char[chars + 1];

    memcpy(string, buffer + location, chars);

    string[chars] = 0;

    location += chars;

    if (location == filesize) isEOF = true;

    return string;
}

long CafeFile::Size()
{
    if (filesize == 0) Read();
    return filesize;
}
