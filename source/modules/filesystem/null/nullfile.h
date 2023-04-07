#ifndef NULL_FILE_H
#define NULL_FILE_H

#include <cstddef>
#include "modules/filesystem/file.h"

class NullFile : public IFile {
private:
    unsigned int fileType_;

public:
    NullFile();
    NullFile(const unsigned int fileType);

    char *Read();

    bool Write(char *data, unsigned int size);

    int ReadInteger();
    float ReadFloat();
    char *ReadString(int chars);

    long Size();
};

#endif // FILE_H
