#include "camera.h"

Camera::Camera()
{
    Init();
    Update();
}

Camera::Camera(float x, float y, float z)
{
    Init();

    position = glm::vec3(x, y, z);
    Update();
}

Camera::Camera(glm::vec3 _position, glm::vec3 _worldUp, float _yaw, float _pitch, float _roll)
{
    Init();

    position = _position;
    worldUp = _worldUp;
    yaw = _yaw;
    pitch = _pitch;
    roll = _roll;

    Update();
}

Camera::Camera(glm::vec3 _position, glm::vec3 _worldUp, glm::vec3 lookAt, float fov)
{
    Init();

    position = _position;
    worldUp = _worldUp;
    forward = lookAt;
    forward = glm::normalize(forward);
    fovDegrees = fov;

    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
    LookAt(right, up, forward);
}

Camera::~Camera()
{
    //viewProjections.RemoveAt(viewProjectionPosition);
    //viewports.RemoveAt(viewportPosition);
    //viewProjections.Clear();
    //viewports.Clear();
}

void Camera::UpdateAfterPhysics()
{

}

void Camera::Update()
{
    LookAt(right, up, forward);
    //viewProjections[viewProjectionPosition] = getProjectionViewMatrix(); // TODO: Check if this actually makes sense??
    viewProjections[0] = getProjectionViewMatrix();
    viewports[0] = viewport;
}

void Camera::LookAt( glm::vec3 right, glm::vec3 up, glm::vec3 forward )
{
    this->right = right;
    this->up = up;
    this->forward = forward;
    view = glm::lookAtRH(position, position + forward, up);
}

Physics::IPhysics::Ray Camera::ScreenPointToRay(float x, float y) const
{
    // Thanks to https://antongerdelan.net/opengl/raycasting.html
    float Px = (2.0f * x) / float(renderer->windowWidth) - 1.0f;
    float Py = 1.0f - (2.0f * y) / float(renderer->windowHeight);
    float Pz = 1.0f;

    glm::vec3 rayNDC(Px, Py, Pz);

    glm::vec4 rayClip(rayNDC.x, rayNDC.y, -1.0, 1.0);

    glm::vec4 rayEye = glm::inverse(getProjectionMatrix()) * rayClip;

    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    glm::vec4 rayWorld = (glm::inverse(view) * rayEye);
    rayWorld = glm::normalize(rayWorld);

    Physics::IPhysics::Ray ray(position, glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));
    return ray;
}

glm::vec2 Camera::WorldPointToScreen(glm::vec3 worldPos, bool &culled ) const
{
    assert( viewPortSize.x == 1024 );
    assert( viewPortSize.y == 768 );
    const float halfWidth = viewPortSize.x / 2.0f;
    const float halfHeight = viewPortSize.y / 2.0f;
    glm::vec2 screen_center(halfWidth, halfHeight); // Needed for vertex transform
    glm::vec4 ndc = glm::vec4( worldPos.x, worldPos.y, worldPos.z, 1.0f );
    glm::vec4 proj = getProjectionViewMatrix() * ndc;
    culled = false;
    if ( proj.x < -proj.w || proj.x > proj.w) culled = true;
    if ( proj.y < -proj.w || proj.y > proj.w) culled = true;
    if ( proj.z < -proj.w || proj.z > proj.w) culled = true;
    proj.x /= proj.w;
    proj.y /= proj.w;
    //proj.z /= proj.w;

    proj.x = screen_center.x + proj.x * halfWidth;
    proj.y = viewPortSize.y - (screen_center.y + proj.y * halfHeight); // Right handed. FlipY.
    return glm::vec2( proj.x, proj.y );
}

glm::mat4 Camera::getProjectionMatrix() const
{
    float aspectXY = viewPortSize.x / viewPortSize.y;
    glm::mat4 projMatrix = glm::perspectiveRH(glm::radians(fovDegrees), aspectXY, nearField, farField);
    return projMatrix;
}

glm::mat4 Camera::getProjectionViewMatrix() const
{
    glm::mat4 projViewMatrix = getProjectionMatrix() * view;
    return projViewMatrix;
}

void Camera::Init()
{
    tag = "Camera";

    fovDegrees = 60.f;
    nearField = 0.1f;
    farField = 100.f;
    position = glm::vec3(0.0f, 0.0f, 0.0f);

    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::cross(forward, up);

    yaw = -90;
    pitch = 0;
    roll = 0;

    SetViewPort(glm::vec2(0,0), glm::vec2(renderer->windowWidth, renderer->windowHeight));

    // TODO: FIX hack!!!
    viewProjections.Clear();
    viewports.Clear();
    // hack !!!!

    viewProjectionPosition = viewProjections.Size();
    viewProjections.Add(getProjectionViewMatrix());

    viewportPosition = viewports.Size();
    viewport = glm::vec4(getViewPortOffset(), getViewPortSize());
    viewports.Add(glm::vec4(viewport));
}
