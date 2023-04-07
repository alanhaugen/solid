#include "keyframe.h"

KeyFrame::KeyFrame()
    : interpolation(0),
      timestamp(0),
      transforms()
{
}

KeyFrame::KeyFrame(int interpolation_, float timestamp_)
    : interpolation(interpolation_),
      timestamp(timestamp_),
      transforms()
{
    transforms.Resize(MAX_JOINTS);
}

void KeyFrame::CalculateTransform()
{
    for (unsigned int i = 0; i < transforms.Size(); i++)
    {
        transforms[i].Scale(transforms[i].scale);
        transforms[i].Rotate(transforms[i].rotation);
        transforms[i].Translate(transforms[i].position);
    }
}
