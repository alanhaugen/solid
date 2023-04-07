#ifndef CAMERA_H
#define CAMERA_H

#include "component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera : public IComponent
{
public:
    Camera();
    Camera(float x, float y, float z);
    Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float roll);
    Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 lookAt, float fov);
    ~Camera();

    void SetViewPort( glm::vec2 offset, glm::vec2 size ) { viewPortOffset = offset; viewPortSize = size; }
    glm::vec2 getViewPortOffset() { return viewPortOffset; }
    glm::vec2 getViewPortSize() { return viewPortSize; }
    void Update();
    void UpdateAfterPhysics();
    void LookAt(glm::vec3 right, glm::vec3 up, glm::vec3 forward);
    IPhysics::Ray ScreenPointToRay( float x, float y ) const;
    glm::vec2 WorldPointToScreen(glm::vec3 worldPos , bool &culled) const;

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getProjectionViewMatrix() const;
    glm::mat4 view;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float roll;
    float nearField;
    float farField;
    float fovDegrees;

private:
    void Init();

    glm::vec2 viewPortOffset; // Pixel Coord offset from 0,0
    glm::vec2 viewPortSize;   // Pixel Coord size (for example 1024x768)

    unsigned int viewProjectionPosition;
    unsigned int viewportPosition;
};

#endif // CAMERA_H
