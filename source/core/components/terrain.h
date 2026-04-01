#ifndef TERRAIN_H
#define TERRAIN_H

#include "component.h"

class Terrain : public Component
{
private:
    int width,height;
    IDrawable* drawable;

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

public:
    Terrain(const char* filePath,
            const char* texturePath = "",
            const char* vertexShaderPath = "shaders/phong.vert.spv",
            const char* fragmentShaderPath = "shaders/phong.frag.spv");
    void Update(float deltaTime);
    void UpdateAfterPhysics();

    float GetHeightAt(float x, float z) const;
    float GetHeightAt(glm::vec3 positionXZ) const;
    glm::vec3 GetNormal(glm::vec3 position) const;
};

#endif // TERRAIN_H