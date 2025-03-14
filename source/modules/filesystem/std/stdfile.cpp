#include "stdfile.h"
#include "core/x-platform/typedefs.h"
#include <sys/stat.h> // For filesize

StdFile::StdFile(String filePath, const unsigned int fileType)
{
    const char *mode = "rb+";

    file = fopen(filePath.ToChar(), mode);

    if (file == NULL)
    {
        LogWarning(String("Failed to open file " + filePath + " (" + filePath + ")"));
    }

    // Get filesize (as recommended here: https://dev.to/namantam1/ways-to-get-the-file-size-in-c-2mag)
    struct stat file_status;

    if (stat(filePath.ToChar(), &file_status) < 0) {
        LogError("Filesize function failed on file " + filePath + "(" + filePath.ToChar() + ")");
    }

    filesize = file_status.st_size;

    buffer = NULL;
}

StdFile::~StdFile()
{
    if(file != NULL)
    {
        fclose(file);
    }

    delete[] buffer;
}

long StdFile::Size()
{
    return filesize;
}

char *StdFile::Read()
{
    if (buffer == NULL)
    {
        buffer = new char[filesize + 1];
        fread(buffer, 1, filesize, file);

        buffer[filesize] = 0;
    }

    return buffer;
}

int StdFile::ReadInteger()
{
    int i;
    fread(&i, sizeof(int), 1, file);

    isEOF = feof(file);

    return i;
}

float StdFile::ReadFloat()
{
    float f = 0.0f;
    fread(&f, sizeof(float), 1, file);

    isEOF = feof(file);

    return f;
}

char *StdFile::ReadString(int chars)
{
    delete[] buffer;

    buffer = new char[chars + 1];
    fread(buffer, sizeof(char), chars, file);

    buffer[chars] = 0;

    isEOF = feof(file);

    return buffer;
}

bool StdFile::Write(char *data, unsigned int size)
{
    UNUSED(data);
    UNUSED(size);
    return false;
}
