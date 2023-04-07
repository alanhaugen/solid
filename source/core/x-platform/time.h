#ifndef TIME_H
#define TIME_H

class ITime {
public:
    virtual float TimeSinceStarted() = 0;

    virtual void Pause() = 0;

    virtual void Reset() = 0;

    virtual ~ITime() {}
};

#endif // TIME_H
