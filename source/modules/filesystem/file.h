#ifndef FILE_H
#define FILE_H

/* // TODO: Check if you can have c++11 support
enum FileType : unsigned int {
    DATA,
    PLAIN_TEXT
};
*/

const unsigned int DATA = 0;
const unsigned int PLAIN_TEXT = 1;

class IFile {
public:
    virtual char *Read() = 0;
    virtual bool Write(char *data, unsigned int size) = 0;
    virtual long Size() = 0;

    virtual int ReadInteger() = 0;
    virtual float ReadFloat() = 0;
    virtual char *ReadString(int chars) = 0;

    int location;
    bool isEOF;

    virtual ~IFile() {}
};

#endif
