#include "glfwtime.h"

GLFWTime::GLFWTime(const char *nameParameter, double timeScaleParameter)
{
    //time = SDL_GetPerformanceCounter();
    name = nameParameter;
    timeScale = timeScaleParameter;
    paused = false;
}

GLFWTime::~GLFWTime()
{
    Log(String(name) + " done: " + String(TimeSinceStarted()) + " ms");
}

float GLFWTime::TimeSinceStarted()
{
    if (paused) return 0;
    //return ((SDL_GetPerformanceCounter() - time)*1000 / (float)SDL_GetPerformanceFrequency());
    return 0;
}

void GLFWTime::Pause()
{
    paused = true;
}

void GLFWTime::Reset()
{
    //time = SDL_GetPerformanceCounter();
    paused = false;
}
