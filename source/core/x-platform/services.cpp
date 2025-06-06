#include "services.h"

IScene *Services::scene = NULL;
Array<IScene *> Services::scenes = Array<IScene *>();
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
        if (scene)
        {
            scene->components.Clear();
        }

        scene = _scene;

        time->Reset();

        scene->Init();

        Log("Loading scene " + String(scene->id) + " done: " + String(time->TimeSinceStarted()) + " ms");

        // Flush input system
        input.Update();
        input.Mouse.Pressed = false;

        // Run the game logic TODO: This is a copy of UpdateScene, duplicate. FIXME: just call UpdateScene
        scene->Update(deltaTime);

        // Update game components
        for (unsigned int i = 0; i < scene->components.Size(); i++)
        {
            (*scene->components[i])->Update();
        }
    }
}

void Services::UpdateScene(IScene *scene)
{
    // Run the game logic
    scene->Update(deltaTime);

    // Update game components
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
        (*scene->components[i])->Update();
    }
}

void Services::UpdateSceneAfterPhysics(IScene *scene)
{
    scene->UpdateAfterPhysics();

    // Update game components after physics
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
        (*scene->components[i])->UpdateAfterPhysics();
    }
}

void Services::UpdateServices()
{
    // Reset frame timer
    time->Reset();

    // Update Input
    input.Update();

    // Update scene
    UpdateScene(scene);

    // Update physics simulation
    physics->Update();

    // Update scene after physics
    UpdateSceneAfterPhysics(scene);

    // Render frame
    renderer->Render(viewProjections, viewports);

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
    scene->id = scenes.Size();
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
