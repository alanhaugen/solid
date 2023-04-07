#ifndef WINDEBUG_H
#define WINDEBUG_H

#include <iostream>
#include "core/x-platform/debug.h"

class WinDebug : public IDebug
{
public:
    WinDebug();
    virtual ~WinDebug() {}
    void Write(const char *text);
    void Error(const char *text);
    void Warning(const char *text);
};

#endif
