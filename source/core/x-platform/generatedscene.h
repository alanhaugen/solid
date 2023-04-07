#ifndef GENERATEDSCENE_H
#define GENERATEDSCENE_H

#include "core/x-platform/scene.h"
#include "core/containers/hash.h"
#include "core/x-platform/parserjson.h"
#include "core/x-platform/animation.h"
#include "core/components/actor.h"
#include "core/components/camera.h"

class GeneratedScene : public IScene
{
public:
    GeneratedScene(String &sceneJSON, int sceneID = -1);
    ~GeneratedScene();

    void Update();
    void UpdateAfterPhysics();

    Hash<String, Animation*> animations;

private:
    void AddCamera();
    void AddPointLight();
    void AddActor();
    void AddDirectional();
    void SetAttenuation();
    void SetAmbient();
    void SetEmission();
    void SetDiffuse();
    void SetShininess();
    void SetSpecular();
    void AddVertex();
    void AddTriangles();
    void AddSphere();
    void SetScale();
    void SetTranslation();
    void SetRotation();
    void AddMesh();
    void PlayVideo();
    void AddAudio();
    void AddAnimation();
    void AddSnowBallCamera();

    IComponent *FindComponent(String tag);

    void Init();

    ParserJSON jsonScene;
    Actor *activeActor;
    Camera *activeCamera;
    Array<IDrawable::Vertex> vertices;
};

#endif // GENERATEDSCENE_H
