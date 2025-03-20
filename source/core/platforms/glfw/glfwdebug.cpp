#include "glfwdebug.h"

GLFWDebug::GLFWDebug() : IDebug()
{
#ifndef NDEBUG
    enabled = true;
#else
    enabled = false;
#endif
}

void GLFWDebug::Write(const char *text)
{
#ifndef NDEBUG
    std::cout << text;
    std::cout << std::endl;
#endif
}

void GLFWDebug::Error(const char *text)
{
#ifdef NDEBUG
#endif

    Write(text);
}

void GLFWDebug::Warning(const char *text)
{
    Write(text);
}

