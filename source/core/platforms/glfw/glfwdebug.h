#ifndef GLFWDEBUG_H
#define GLFWDEBUG_H

#include <iostream>
#include <cstddef>
#include "core/x-platform/string.h"
#include "core/x-platform/debug.h"

class GLFWDebug : public IDebug
{
private:

public:
    GLFWDebug();

    virtual void Write(const char *text);
    virtual void Error(const char *text);
    virtual void Warning(const char *text);
};

#endif
