#ifndef SDL_DEBUG_H
#define SDL_DEBUG_H

#include <SDL.h>
#include <iostream>
#include <cstddef>
#include "core/x-platform/string.h"
#include "core/x-platform/debug.h"

class SDLDebug : public IDebug
{
private:
    SDL_Window *window;

public:
    SDLDebug();

    void SetDisplay(SDL_Window *display);

    virtual void Write(const char *text);
    virtual void Error(const char *text);
    virtual void Warning(const char *text);
};

#endif
