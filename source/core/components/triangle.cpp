#include "triangle.h"

Triangle::Triangle()
{
    Init(IDrawable::Vertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
         IDrawable::Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
         IDrawable::Vertex(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
}

Triangle::Triangle(IDrawable::Vertex v1, IDrawable::Vertex v2, IDrawable::Vertex v3)
{
    Init(v1, v2, v3);
}

void Triangle::Update()
{
    //uniforms.Lock();
    //drawable->uniformData = uniforms;
    drawable->matrix = matrix.subMatrix * matrix.matrix;

    renderer->Draw(drawable);
}

void Triangle::UpdateAfterPhysics()
{
}

void Triangle::Init(IDrawable::Vertex v1, IDrawable::Vertex v2, IDrawable::Vertex v3)
{
    tag = "triangle";

    Array<IDrawable::Vertex> vertices(3);
    Array<unsigned int> indices;
    Array<String> shaders(2);

    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    *matrix.x = 0.0f;
    *matrix.y = 0.0f;
    *matrix.z = 0.0f;

    shaders[VERTEX_SHADER]   = "data/simple.vert";
    shaders[FRAGMENT_SHADER] = "data/simple.frag";

    drawable = renderer->CreateDrawable(vertices, indices, shaders);

    matrix.position = glm::vec3(0.0f, 0.0f, 0.0f);
    drawable->matrix = matrix.matrix;
}
