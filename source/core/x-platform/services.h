#ifndef SERVICES_H
#define SERVICES_H

#include "core/x-platform/scene.h"
#include "core/x-platform/generatedscene.h"

class Services : public Locator
{
public:
    static void SetScene(IScene *scene);

    void UpdateServices();

    static void NextScene();

    static void LoadScene(const unsigned int scene);

    void AddScene(IScene *scene);

    void AddScene(String sceneJSON, int id = -1);

    static void Quit();

    static IScene *scene;

    static bool isAlive;

    static Array<Ptr<IScene *> > scenes;

    static Array<String> sceneJSONs;
};

#endif // SERVICES_H
