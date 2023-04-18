#include "sdlrender.h"

SDLRender::SDLRender()
{
}

SDLRender::~SDLRender()
{
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
}

bool SDLRender::Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight)
{
#ifdef __RASPBERRYPI__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN;
    }

    mainwindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowLength, windowHeight, flags);

    if (mainwindow == NULL)
    {
        LogError(SDL_GetError());
        return false;
    }

    // Create our opengl context and attach it to our window
    maincontext = SDL_GL_CreateContext(mainwindow);

    if (maincontext == NULL)
    {
        Log(SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

    if (version == 0)
    {
        LogError("GLAD failed to load OpenGL");
        return false;
    }

    Log("OpenGL " + String(GLAD_VERSION_MAJOR(version)) + "." + String(GLAD_VERSION_MINOR(version)));

#ifndef __WEBASM__
    if (GLAD_VERSION_MAJOR(version) < 3)
    {
        LogError("OpenGL core profile failed to load");
        return false;
    }
#endif

    return RENDERER::Init(fullscreen, windowTitle, windowLength, windowHeight);
}

void SDLRender::PreRender()
{
}

void SDLRender::PostRender()
{
    SDL_GL_SwapWindow(mainwindow);
}

SDL_Window *SDLRender::GetDisplay()
{
    return mainwindow;
}
