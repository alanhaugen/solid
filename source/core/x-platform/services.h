#ifndef SERVICES_H
#define SERVICES_H

#include "core/x-platform/scene.h"
#include "core/x-platform/generatedscene.h"

static double FRAME_TIME_MS = 1000.f / 60.f;

class Services : public Locator
{
public:
    static void SetScene(IScene *scene);

    void UpdateServices();

    static void NextScene();

    static void LoadScene(const unsigned int scene);

    void AddScene(IScene *scene);

    void AddScene(String sceneJSON, int id = -1);

    void UpdateScene(IScene *scene);

    void UpdateSceneAfterPhysics(IScene *scene);

    static void Quit();

    static IScene *scene;

    static bool isAlive;

    static Array<IScene *> scenes;

    static Array<String> sceneJSONs;
};

#endif // SERVICES_H
