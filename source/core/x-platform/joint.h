#ifndef JOINT_H
#define JOINT_H

#include "glm/glm.hpp"
#include "core/x-platform/string.h"

//! https://www.youtube.com/watch?v=F-kcaonjHf8

class Joint
{
private:

public:
    String name;
    glm::mat4 worldBindTransform;
    glm::mat4 inverseBindTransform;

    Joint(int _id, String _jointName, glm::mat4 _bindTransform);
    void CalculateInverseTransform(glm::mat4 parentWorldransform);

    int id;
    Array<Joint*> children;
};

#endif // JOINT_H
