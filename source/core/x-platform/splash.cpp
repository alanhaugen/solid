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
    components.Add(new Sprite("data/logo_inv.png"));

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
