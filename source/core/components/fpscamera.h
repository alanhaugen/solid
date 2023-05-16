#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include "component.h"
#include "camera.h"

class FPSCamera : public Component
{
private:
    Camera *camera;
    float speed;

public:
    FPSCamera(Camera *_camera);

    void Update();
    void UpdateAfterPhysics();

    void UpdateCamera();
};

#endif // FPSCAMERA_H
