#ifndef DEBUG_H
#define DEBUG_H

//! Used to print debug messages and track objects
class IDebug {
protected:
    bool enabled;

public:
    virtual ~IDebug() {};
    virtual void Write(const char *text) = 0;
    virtual void Error(const char *text) = 0;
    virtual void Warning(const char *text) = 0;
};

#endif
