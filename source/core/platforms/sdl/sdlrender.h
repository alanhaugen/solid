#ifndef SDLRENDER_H
#define SDLRENDER_H

#include <SDL.h>
#include "core/containers/array.h"
#include "core/x-platform/computedinclude.h"
#include COMPUTED_INCLUDE(RENDERER_HEADER)

class SDLRender : public RENDERER
{
private:
    SDL_Window* mainwindow;
    SDL_GLContext maincontext;

public:
    SDLRender();
    ~SDLRender();

    bool Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight);

    void PreRender();

    void PostRender();

    SDL_Window *GetDisplay();
};

#endif // SDLRENDER_H
