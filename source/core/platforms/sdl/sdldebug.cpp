#include "sdldebug.h"
#ifdef NDK
#include <android/log.h>
#endif

SDLDebug::SDLDebug() : IDebug()
{
    window = NULL;
#ifndef NDEBUG
    enabled = true;
#else
    enabled = false;
#endif
}

void SDLDebug::SetDisplay(SDL_Window *windowParameter)
{
    window = windowParameter;
}

void SDLDebug::Write(const char *text)
{
#ifndef NDEBUG
    std::cout << text;
    std::cout << std::endl;
#ifdef NDK
    __android_log_print(ANDROID_LOG_DEBUG, "Solid", "Log: %s", text);
#endif
#endif
}

void SDLDebug::Error(const char *text)
{
#ifdef NDEBUG
    if (window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", (String("Please contact alanhaugen@gmail.com") + text).ToChar(), window);
    }
#endif

    Write(text);
}

void SDLDebug::Warning(const char *text)
{
    Write(text);
}
