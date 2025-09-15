#ifndef MAT_H
#define MAT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class Mat
{
public:
    Mat();
    Mat(glm::mat4 matrix);
    float *x, *y, *z;

    Mat operator* (Mat x)
    {
        return x.matrix * matrix;
    }

    //void Rotation();
    //void Scale();
    void Translate(glm::vec3);
    void SetPosition(glm::vec3);
    //void Translate(glm::vec4);

    void Scale(glm::vec3);

    void Rotate(float angleDegrees, glm::vec3);
    void Rotate(glm::vec4);
    //void Rotate(glm::mat4);
    void Rotate(glm::quat quat);

    glm::mat4 matrix;
    glm::mat4 subMatrix;

    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 scale;
private:
    void SetXYZ();
};

#endif // MAT_H
