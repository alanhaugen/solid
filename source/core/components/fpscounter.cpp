#include "fpscounter.h"
#include "core/application.h"

FPSCounter::FPSCounter()
{
    counter = Application::GetTime("FPS");
}

void FPSCounter::UpdateAfterPhysics()
{

}

void FPSCounter::Update()
{
    float hz = 1.0f / counter->TimeSinceStarted() * 1000.0f; // 1 frame / elapsed sec
    Log("FPS: " + String(hz));

    counter->Reset();
}
