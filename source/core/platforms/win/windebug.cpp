#include "windebug.h"
#include <windows.h>

WinDebug::WinDebug() : IDebug()
{
#ifndef NDEBUG
    enabled = true;
#else
    enabled = false;
#endif
}

void WinDebug::Write(const char *text)
{
#ifndef NDEBUG
    std::cout << text;
    std::cout << std::endl;
#endif
}

void WinDebug::Error(const char *text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9); // set text color to bright red

#ifdef NDEBUG
    MessageBoxA(0, text, "Error",0);
#endif

    Write(text);
}

void WinDebug::Warning(const char *text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15); // set text color to bright yellow

    Write(text);
}
