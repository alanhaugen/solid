#include "wintime.h"

WinTime::WinTime(const char *name_, double timeScale_)
{
    Reset();
    name = name_;
    timeScale = timeScale_;
}

WinTime::~WinTime()
{
    Log(String(name) + " done: " + String(TimeSinceStarted()) + " ms");
}

float WinTime::TimeSinceStarted()
{
    if (paused)
    {
        return 0;
    }

    currentTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(currentTime - lastTime).count() * timeScale - amountPaused;
}

void WinTime::Pause()
{
    paused = !paused;

    if(paused)
    {
        pauseTime = std::chrono::high_resolution_clock::now();
    }
    else
    {
        amountPaused += std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - pauseTime).count() * timeScale;
    }
}

void WinTime::Reset()
{
    lastTime = std::chrono::high_resolution_clock::now();
    paused = false;
    amountPaused = 0;
}
