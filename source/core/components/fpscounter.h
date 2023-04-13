#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "core/components/component.h"
#include "core/components/text.h"
#include "core/x-platform/time.h"

class FPSCounter : public IComponent
{
public:
    FPSCounter();
    ~FPSCounter();
    ITime *counter;
    Text *text;

    void Update();
    void UpdateAfterPhysics();
};

#endif // FPSCOUNTER_H
