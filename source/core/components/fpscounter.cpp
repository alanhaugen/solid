#include "fpscounter.h"
#include "core/application.h"

FPSCounter::FPSCounter()
{
    counter = Application::GetTime("FPS");
    //text = new Text("FPS");
}

FPSCounter::~FPSCounter()
{
    delete counter;
    delete text;
}

void FPSCounter::UpdateAfterPhysics()
{
}

void FPSCounter::Update()
{
    float hz = 1.0f / counter->TimeSinceStarted() * 1000.0f; // 1 frame / elapsed sec

    //delete text;
    //text = new Text("FPS: " + String(hz));
    //text->Update();

    Log(String(String(hz)));

    counter->Reset();
}
