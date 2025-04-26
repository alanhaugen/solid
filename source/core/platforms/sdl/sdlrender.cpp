#include "sdlrender.h"
#include <core/application.h>

#ifdef USE_VULKAN
#include <SDL_vulkan.h>
#else
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#endif
#endif

SDLRender::SDLRender()
{
}

SDLRender::~SDLRender()
{
#ifndef USE_VULKAN
    SDL_GL_DeleteContext(maincontext);
#endif
    SDL_DestroyWindow(mainwindow);
}

bool SDLRender::Init(bool fullscreen, const char *windowTitle, const unsigned int windowLength, const unsigned int windowHeight)
{
#ifdef USE_VULKAN
    unsigned int flags = SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN;
#else
#ifdef ES_PROFILE
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // TODO: Test with 32-bit

    unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
#endif

    /*if (fullscreen)
    {
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP; //SDL_WINDOW_FULLSCREEN;
    }*/

    mainwindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowLength, windowHeight, flags);

    if (mainwindow == NULL)
    {
        LogError(SDL_GetError());
        return false;
    }

#ifdef USE_VULKAN
    unsigned int extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(mainwindow, &extensionCount, nullptr);

    extensionNames.resize(extensionCount);

    SDL_Vulkan_GetInstanceExtensions(mainwindow, &extensionCount, extensionNames.data());

    int width  = 0;
    int height = 0;

    bool success = RENDERER::Init(fullscreen, windowTitle, windowLength, windowHeight);

    if (success)
    {
        success = SDL_Vulkan_CreateSurface(mainwindow, instance, &surface);

        if (success == false)
        {
            LogError("Failed to create vulkan surface.");
        }

        // It is the queues that perform the work that your application requests
        SelectQueueFamily();

        // Create device
        success = CreateDevice();

        if (success == false)
        {
            LogError("Failed to create vulkan device.");
            return false;
        }

        SDL_Vulkan_GetDrawableSize(mainwindow, &width, &height);
        CreateSwapChain(width, height); // For backbuffering
        success = SetupScreenAndCommand();
    }

    return success;
#else
    // Create our opengl context and attach it to our window
    maincontext = SDL_GL_CreateContext(mainwindow);

    if (maincontext == NULL)
    {
        LogError(SDL_GetError());
        return false;
    }

    // https://wiki.libsdl.org/SDL2/SDL_GL_SetSwapInterval
    // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync
    SDL_GL_SetSwapInterval(0); // Try with 1 to remove tearing?

    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

    if (version == 0)
    {
        LogError("GLAD failed to load OpenGL");
        return false;
    }

    Log("OpenGL " + String(GLAD_VERSION_MAJOR(version)) + "." + String(GLAD_VERSION_MINOR(version)));

#ifndef __WEBASM__ 
    /*if (GLAD_VERSION_MAJOR(version) < 3)
    {
        LogError("OpenGL core profile failed to load");
        return false;
    }*/
#endif
#endif

    return RENDERER::Init(fullscreen, windowTitle, windowLength, windowHeight);
}

void SDLRender::PreRender()
{
#ifndef USE_VULKAN
#ifdef __APPLE__
    GLint                       sync = 0;
    CGLContextObj               ctx = CGLGetCurrentContext();

    CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
#endif
}

void SDLRender::PostRender()
{
    SDL_GL_SwapWindow(mainwindow);
}

SDL_Window *SDLRender::GetDisplay()
{
    return mainwindow;
}
