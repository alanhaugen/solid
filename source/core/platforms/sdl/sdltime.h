#ifndef SDLTIME_H
#define SDLTIME_H

#include <SDL.h>
#include "core/x-platform/locator.h"
#include "core/x-platform/time.h"

class SDLTime : public ITime
{
private:
    bool paused;
    Uint64 time;
    const char *name;
    float timeScale;

public:
    SDLTime(const char *nameParameter, double timeScaleParameter = 1.0);
    ~SDLTime();

    float TimeSinceStarted();

    void Pause();

    void Reset();
};

#endif // SDLTIME_H
