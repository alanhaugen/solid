#include "joint.h"

Joint::Joint(int _id, String _jointName, glm::mat4 _bindTransform)
{
    id = _id;
    name = _jointName;
    worldBindTransform = _bindTransform;
}

void Joint::CalculateInverseTransform(glm::mat4 parentWorldTransform)
{
    glm::mat4 localBindTransform = glm::inverse(parentWorldTransform) * worldBindTransform;
    glm::mat4 bindTransform = parentWorldTransform * localBindTransform;

    inverseBindTransform = glm::inverse(bindTransform);

    for (unsigned int i = 0; i < children.Size(); i++)
    {
        children[i]->CalculateInverseTransform(bindTransform);
    }
}
