#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "core/x-platform/joint.h"
#include "core/x-platform/locator.h"
#include "core/x-platform/mat.h"

class KeyFrame
{
private:
    int interpolation;
    float timestamp;

public:
    //Array<Joint> joints;
    Array<Mat> transforms; // Transforms in animation

    KeyFrame();
    KeyFrame(int interpolation, float timestamp);

    void CalculateTransform();
};

#endif // KEYFRAME_H
