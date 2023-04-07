#include "sdltime.h"

SDLTime::SDLTime(const char *nameParameter, double timeScaleParameter)
{
    time = SDL_GetPerformanceCounter();
    name = nameParameter;
    timeScale = timeScaleParameter;
    paused = false;
}

SDLTime::~SDLTime()
{
    Log(String(name) + " done: " + String(TimeSinceStarted()) + " ms");
}

float SDLTime::TimeSinceStarted()
{
    if (paused) return 0;
    return ((SDL_GetPerformanceCounter() - time)*1000 / (float)SDL_GetPerformanceFrequency());
}

void SDLTime::Pause()
{
    paused = true;
}

void SDLTime::Reset()
{
    time = SDL_GetPerformanceCounter();
    paused = false;
}
