#include "splash.h"
#include "core/application.h"
#include "core/components/camera.h"

Splash::Splash()
{
}

Splash::~Splash()
{
}

void Splash::Init()
{
    components.Add(new Camera());
    components.Add(new Text("SOLID ENGINE V. 1.0"));
    components.Add(new Sprite("data/logo.png",
                              (renderer->windowWidth/2.0f) - 124.0f,
                              (renderer->windowHeight/2.0f) - 36.0f,
                              3.0f,
                              3.0f));

    timer = Application::GetTime();

    // Set scene ID
    id = Application::scenes.Size();
}

void Splash::Update()
{
    if (timer->TimeSinceStarted() > 2000)
    {
        Application::NextScene();
    }
}

void Splash::UpdateAfterPhysics()
{
}
