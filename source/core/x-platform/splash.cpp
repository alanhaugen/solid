#include "splash.h"
#include "core/application.h"
#include "core/components/camera.h"
#include "core/components/background.h"
#include "core/components/sprite.h"

Splash::Splash()
{
}

Splash::~Splash()
{
}

void Splash::Init()
{
    components.Add(new Camera);
    components.Add(new Sprite("data/logo.png",
                              (renderer->windowWidth/2.0f) - 124.0f,
                              (renderer->windowHeight/2.0f) - 36.0f,
                              1.0f,
                              1.0f));
    components.Add(new Background(glm::vec3(1,1,1)));

    timer = Application::GetTime();

    // Set scene ID
    id = Application::scenes.Size();
}

void Splash::Update(float dt)
{
    if (timer->TimeSinceStarted() > 2000.0f)
    {
        Application::NextScene();
    }
}

void Splash::UpdateAfterPhysics()
{
}
