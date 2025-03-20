#ifndef GLFWTIME_H
#define GLFWTIME_H

#include "core/x-platform/locator.h"
#include "core/x-platform/time.h"

class GLFWTime : public ITime
{
private:
    bool paused;
    Uint64 time;
    const char *name;
    float timeScale;

public:
    GLFWTime(const char *nameParameter, double timeScaleParameter = 1.0);
    ~GLFWTime();

    float TimeSinceStarted();

    void Pause();

    void Reset();
};

#endif // GLFWTIME_H
