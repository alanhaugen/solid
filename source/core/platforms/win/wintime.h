#ifndef WINTIME_H
#define WINTIME_H

#include <chrono>
#include "core/x-platform/locator.h"
#include "core/x-platform/time.h"

class WinTime : public ITime
{
private:
    bool paused;
    const char *name;
    double timeScale;
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

public:
    WinTime(const char *name_, double timeScale_ = 1.0);
    ~WinTime();

    float TimeSinceStarted();

    void Pause();

    void Reset();
};

#endif // WINTIME_H
