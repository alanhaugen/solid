#include "primitives.h"

Primitives::Primitives()
{
    IDrawable::Vertex vertex1;
    vertex1.position = glm::vec3(-0.5f, -0.5f, 1.0f);
    vertex1.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex2;
    vertex2.position = glm::vec3(0.5f, -0.5f, 1.0f);
    vertex2.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex3;
    vertex3.position = glm::vec3(0.5f, 0.5f, 1.0f);
    vertex3.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex4;
    vertex4.position = glm::vec3(-0.5f, 0.5f, 1.0f);
    vertex4.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex5;
    vertex5.position = glm::vec3(-0.5f, 0.5f, -1.0f);
    vertex5.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex6;
    vertex6.position = glm::vec3(-0.5f, -0.5f, -1.0f);
    vertex6.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex7;
    vertex7.position = glm::vec3(0.5f, 0.5f, -1.0f);
    vertex7.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    IDrawable::Vertex vertex8;
    vertex8.position = glm::vec3(0.5f, -0.5f, -1.0f);
    vertex8.color    = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    Array<unsigned int> indices_cube(8);
    indices_cube[0] = 0;
    indices_cube[1] = 1;
    indices_cube[2] = 2;
    indices_cube[3] = 2;
    indices_cube[4] = 3;
    indices_cube[5] = 0;

    Array<unsigned int> indices_triangle(3);
    indices_triangle[0] = 0;
    indices_triangle[1] = 1;
    indices_triangle[2] = 2;

    Array<IDrawable::Vertex> vertices(8);
    vertices[0] = vertex1;
    vertices[1] = vertex2;
    vertices[2] = vertex3;
    vertices[3] = vertex4;
    vertices[4] = vertex5;
    vertices[5] = vertex6;
    vertices[6] = vertex7;
    vertices[7] = vertex8;

    cube.Add(new Mesh(vertices, indices_cube));
    triangle.Add(new Mesh(vertices, indices_triangle));
    car.Add(new Mesh(URL("data/car1.3DS")));

    cube.x += 10;
    cube.z = -10; // move infront of camera
}

void Primitives::Update(Input *input)
{
}

void Primitives::UpdateLate(Input *input)
{
}
