#include "splash.h"
#include "core/application.h"
#include "core/components/camera.h"

Splash::Splash()
{
    Camera *camera = new Camera();

    components.Add(new Sprite(URL("data/logo_inv_jpg.jpg")));
    components.Add(camera);

    timer = Application::GetTime();

    // Set scene ID
    id = Application::scenes.Size();
}

Splash::~Splash()
{
    // Delete pointers in array
    for (unsigned int i = 0; i < components.Size(); i++)
    {
        delete components[i];
        components[i] = NULL;
    }
}

void Splash::Update()
{
    if (timer->TimeSinceStarted() > 2000)
    {
        Application::NextScene();
    }
}

void Splash::UpdateLate()
{
}
