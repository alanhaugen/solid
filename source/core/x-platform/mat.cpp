#include "mat.h"

Mat::Mat()
    : matrix(glm::mat4()),
      subMatrix(glm::mat4())
{
    rotation = glm::quat();
    position = glm::vec3();
    scale = glm::vec3(1.0f);

    SetXYZ();
}

Mat::Mat(glm::mat4 matrix_)
    : matrix(matrix_),
      subMatrix(glm::mat4())
{
    rotation = glm::quat();
    position = glm::vec3();
    scale = glm::vec3(1.0f);

    SetXYZ();
}

void Mat::Translate(glm::vec3 pos)
{
    matrix = glm::translate(glm::mat4(), pos) * matrix;

    SetXYZ();
}

/*void Mat::Translate(glm::vec4 pos)
{
    //matrix[3] += pos; // Not correct???
}*/

void Mat::Scale(glm::vec3 scale)
{
    matrix = glm::scale(matrix, scale) * matrix;

    SetXYZ();
}

void Mat::Rotate(float angleDegrees, glm::vec3 rotation)
{
    matrix = glm::rotate(matrix, angleDegrees, rotation); //glm::radians(degrees)

    SetXYZ();
}

void Mat::Rotate(glm::vec4 rotation)
{
    glm::quat quat(rotation.w, rotation.x, rotation.y, rotation.z);

    matrix = glm::mat4_cast(quat) * matrix;

    SetXYZ();
}

void Mat::Rotate(glm::quat quat)
{
    matrix = glm::mat4_cast(quat) * matrix;

    SetXYZ();
}

void Mat::SetXYZ()
{
    position.x = matrix[3].x;
    position.y = matrix[3].y;
    position.z = matrix[3].z;

    x = &matrix[3].x;
    y = &matrix[3].y;
    z = &matrix[3].z;
}

/*void Mat::Rotate(glm::mat4 rotation)
{
    matrix *= rotation;
}*/
