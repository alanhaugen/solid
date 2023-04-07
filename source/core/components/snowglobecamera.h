#ifndef SNOWGLOBECAMERA_H
#define SNOWGLOBECAMERA_H

#include "component.h"
#include "camera.h"
#include "actor.h"

# define PI           3.14159265358979323846

class SnowGlobeCamera : public IComponent
{
private:
    Actor *target;
    Camera *camera;

    float speed;

    glm::mat3 Rotate(float degrees, glm::vec3 axis);                // const float degrees, const vec3& axis
    void Up(float degrees);       // float degrees, vec3& eye, vec3& up
    void Left(float degrees);     // float degrees, vec3& eye, vec3& up

public:
    SnowGlobeCamera();
    SnowGlobeCamera(Actor *target_, Camera *camera_);

    void Update();
    void UpdateAfterPhysics();
};

#endif // SNOWGLOBECAMERA_H
