#include "nullfile.h"

NullFile::NullFile()
{
    location = 0;
    isEOF = false;
}

NullFile::NullFile(const unsigned int fileType)
{
    fileType_ = fileType;
}

bool NullFile::Write(char *data, unsigned int size)
{
    (void)data;
    (void)size;
    return false;
}

char *NullFile::Read()
{
    if (fileType_ == PLAIN_TEXT) {
        return const_cast<char *>("");
    }

    return NULL;
}

int NullFile::ReadInteger()
{
    return 0;
}

float NullFile::ReadFloat()
{
    return 0;
}

char *NullFile::ReadString(int chars)
{
    return const_cast<char *>("");
}

long NullFile::Size()
{
    return 0;
}
