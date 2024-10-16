#ifndef MESH_H
#define MESH_H

#include "component.h"
#include "core/x-platform/url.h"
#include "core/x-platform/animation.h"
#include "core/x-platform/joint.h"
#include "modules/renderer/drawable.h"
#include "core/components/actor.h"

#define _USE_MATH_DEFINES
#include "glm/glm.hpp"

#include <vector>

class Mesh : public Component
{
private:
    Array<IDrawable*> drawables;
    Joint *rootJoint;

    ITime *framerate;

    //Animation *animation;
    //Animation *emptyAnimation;

    //void LoadFromGLTF(const URL filePath);
    void LoadFromWavefront(const URL &filePath);
    void LoadFrom3DFile(const String &filePath, const String vertexShaderPath = "data/simple.vert", const String fragmentShaderPath = "data/simple.frag");

    float time; // TODO: REMOVE when obj animation hack is obsolete
    //Array<Animation*> animations;

public:
    Mesh(const String &type, glm::vec4 vertexColor);
    Mesh(const String &filePath, const String vertexShaderPath = "data/simple.vert", const String fragmentShaderPath = "data/simple.frag");
    Mesh(Array<IDrawable::Vertex> &vertices, Array<unsigned int> &indices, Actor *actor = NULL);
    Mesh(const Mesh &other);
    Mesh();
    ~Mesh();

    void Update();
    void UpdateAfterPhysics();

    void Hide();
    void Show();

    void Scale();
    void Rotate();

    void PlayAnimation(Animation *anim, int blend = 1.0, bool reset = false);
};

#endif    // MESH_H
