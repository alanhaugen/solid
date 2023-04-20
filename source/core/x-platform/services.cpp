#include "services.h"

IScene *Services::scene = NULL;
Array<IScene*> Services::scenes = Array<IScene*>();
bool Services::isAlive = true;

void Services::SetScene(IScene *_scene)
{
    if (_scene == NULL)
    {
        LogWarning("Can not find scene");
    }
    else if (_scene == scene)
    {
        LogWarning("Scene is already loaded");
    }
    else
    {
        delete scene;
        scene = _scene;
        scene->Init();
    }
}

void Services::UpdateServices()
{
    // Reset frame timer
    time->Reset();

    // Update Input
    input.Update();

    // Update game components
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
        scene->components[i]->Update();
    }

    // Run the game logic
    scene->Update();

    // Render frame
    renderer->Render(viewProjections, viewports);

    // Update physics simulation
    physics->Update();

    // Update logic done after physics
    scene->UpdateAfterPhysics();

    // Update game components after physics
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
        scene->components[i]->UpdateAfterPhysics();
    }

    // Update sound system
    audio->Update();
}

void Services::LoadScene(const unsigned int scene)
{
    SetScene(scenes[scene]);
}

void Services::NextScene()
{
    if (scene->id+1 < scenes.Size())
    {
        SetScene(scenes[scene->id+1]);
    }
}

void Services::AddScene(IScene *scene)
{
    scenes.Add(scene);

    // Load the first scene
    if (scenes.Size() == 1)
    {
        LoadScene(0);
    }
}

void Services::AddScene(String sceneJSON, int id)
{
    IScene *scene = new GeneratedScene(sceneJSON, id);

    AddScene(scene);
}

void Services::Quit()
{
    isAlive = false;
}
