#include "fpscamera.h"

FPSCamera::FPSCamera(Camera *_camera)
{
    camera = _camera;
    speed = 1.0;
}

void FPSCamera::UpdateAfterPhysics()
{
}

void FPSCamera::Update()
{
    if (input.Mouse.Down)
    {
        if (input.Held(input.Key.W))
        {
            camera->position += camera->forward * speed * deltaTime;
        }
        if (input.Held(input.Key.S))
        {
            camera->position -= camera->forward * speed * deltaTime;
        }
        if (input.Held(input.Key.D))
        {
            camera->position += camera->right * speed * deltaTime;
        }
        if (input.Held(input.Key.A))
        {
            camera->position -= camera->right * speed * deltaTime;
        }

        camera->yaw   += input.Mouse.dx;
        camera->pitch -= input.Mouse.dy;

        input.Mouse.Dragging = true;

        UpdateCamera();
    }
    else
    {
        input.Mouse.Dragging = false;
    }
}

void FPSCamera::UpdateCamera()
{
    if (camera->pitch > 89.0f)
    {
        camera->pitch = 89.0f;
    }
    else if (camera->pitch < -89.0f)
    {
        camera->pitch = -89.0f;
    }

    camera->forward.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->forward.y = sin(glm::radians(camera->pitch));
    camera->forward.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->forward = glm::normalize(camera->forward);

    camera->right = glm::normalize(glm::cross(camera->forward, camera->worldUp));
    camera->up = glm::normalize(glm::cross(camera->right, camera->forward));
}
