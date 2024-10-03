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
        scene->Init();

        // Run the game logic TODO: This is a copy of UpdateScene, duplicate. FIXME: just call UpdateScene
        scene->Update();

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
    scene->Update();

    // Update game components
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
        (*scene->components[i])->Update();
    }
}

void Services::UpdateSceneAfterPhysics(IScene *scene)
{
    // Update game components after physics
    for (unsigned int i = 0; i < scene->components.Size(); i++)
    {
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

    // Render frame
    renderer->Render(viewProjections, viewports);

    // Update physics simulation
    physics->Update();

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
