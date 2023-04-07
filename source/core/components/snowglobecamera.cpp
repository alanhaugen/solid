#include "snowglobecamera.h"

SnowGlobeCamera::SnowGlobeCamera()
{

}

SnowGlobeCamera::SnowGlobeCamera(Actor *target_, Camera *camera_)
{
    target = target_;
    camera = camera_;
}

void SnowGlobeCamera::UpdateAfterPhysics()
{
}

void SnowGlobeCamera::Update()
{
    if (input.Mouse.Down)
    {
        if (input.Held(input.Key.W))
        {
            target->matrix.Translate(glm::vec3(camera->forward * speed * deltaTime));
        }
        if (input.Held(input.Key.S))
        {
            target->matrix.Translate(glm::vec3(camera->forward * speed * deltaTime));
        }
        if (input.Held(input.Key.D))
        {
            target->matrix.Translate(glm::vec3(camera->right * speed * deltaTime));
        }
        if (input.Held(input.Key.A))
        {
            target->matrix.Translate(glm::vec3(camera->right * speed * deltaTime));
        }

        Left(input.Mouse.dx);
        Up(input.Mouse.dy);
    }
}

glm::mat3 SnowGlobeCamera::Rotate(float degrees, glm::vec3 axis)
{
    const double radians = degrees * (PI/180);

    glm::mat3 matrix = float(cos(radians)) * glm::mat3(glm::vec3(1, 0, 0),
                                                       glm::vec3(0, 1, 0),
                                                       glm::vec3(0, 0, 1)) +
            (1 - float(cos(radians))) * glm::mat3(glm::vec3(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z),
                                                  glm::vec3(axis.x * axis.y, axis.y * axis.y, axis.y * axis.z),
                                                  glm::vec3(axis.x * axis.z, axis.y * axis.z, axis.z * axis.z)) +
            float(sin(radians)) * glm::mat3(glm::vec3(0, -axis.z, axis.y),
                                            glm::vec3(axis.z, 0, -axis.x),
                                            glm::vec3(-axis.y, axis.x, 0));

    matrix = glm::transpose(matrix);

    return matrix;
}

void SnowGlobeCamera::Up(float degrees)
{
    glm::vec3 forward = camera->forward;
    glm::vec3 right   = glm::cross(forward, camera->up);
    right = normalize(right);

    glm::mat3 matrix = Rotate(degrees, right);

    camera->up = matrix * camera->up;
    camera->forward = matrix * camera->forward;
}

void SnowGlobeCamera::Left(float degrees)
{
    camera->forward = camera->forward * Rotate(-degrees, camera->up);
}

/*
left
// Transforms the camera left around the "crystal ball" interface
    // YOUR CODE FOR HW1 HERE
    eye = eye * rotate(-degrees, up);

up
// Transforms the camera up around the "crystal ball" interface
    // YOUR CODE FOR HW1 HERE
    vec3 forward = eye;
    vec3 right   = cross(forward, up);
    right = normalize(right);

    mat3 matrix = rotate(degrees, right);

    up  = matrix * up;
    eye = matrix * eye;

rotate
// Helper rotation function.
    // YOUR CODE FOR HW1 HERE
    const float radians = degrees * (pi/180);

    mat3 matrix = cos(radians) * mat3(1,0,0,0,1,0,0,0,1) + (1 - cos(radians)) * mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z, axis.x*axis.y, axis.y*axis.y, axis.y*axis.z, axis.x*axis.z, axis.y*axis.z, axis.z*axis.z) + sin(radians) * mat3(0, -axis.z, axis.y, axis.z, 0, -axis.x, -axis.y, axis.x, 0);

    matrix = transpose(matrix);

    return matrix;

lookat
// Your implementation of the glm::lookAt matrix
    // YOUR CODE FOR HW1 HERE
    const vec3 center = vec3(0, 0, 0);

    const vec3 a = eye - center;
    const vec3 b = up;

    const vec3 w = normalize(a);

    const vec3 u = normalize(cross(b, w));
    const vec3 v = cross(w, u);

    mat4 transform = mat4(	u.x, u.y, u.z, -u.x * eye.x - u.y * eye.y - u.z * eye.z,
                v.x, v.y, v.z, -v.x * eye.x - v.y * eye.y - v.z * eye.z,
                w.x, w.y, w.z, -w.x * eye.x - w.y * eye.y - w.z * eye.z,
                0,   0,   0,   1);

    transform = transpose(transform);

    return transform;
*/
