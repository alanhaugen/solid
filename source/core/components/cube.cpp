#include "cube.h"

Cube::Cube(float x, float y, float z, float length, float width, float height, glm::vec4 color, String shaderVertPath, String shaderFragPath)
{
    *matrix.x = x;
    *matrix.y = y;
    *matrix.z = z;

    matrix.position = glm::vec3(x,y,z);

    Array<String> shaders(2);

    shaders[VERTEX_SHADER]   = shaderVertPath;
    shaders[FRAGMENT_SHADER] = shaderFragPath;

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

    // 36 coords for a cube
    // Thanks to Dr. Anton https://antongerdelan.net/opengl/cubemaps.html
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(0,1)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,1)));

    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(0,1)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,1)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width), color, glm::vec2(0,1)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(0,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width), color, glm::vec2(1,0)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(1,1)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height,  1.0f*width), color, glm::vec2(0,1)));

    tag = "cube";
    ITexture* texture = renderer->CreateTexture("data/block.png");
    drawable = renderer->CreateDrawable(vertices, indices, shaders, texture);
    collisionBox = physics->CreateHitBox(glm::vec3(0.5f), &matrix, tag);
    drawable->matrix = matrix.matrix;
}

Cube::~Cube()
{
    renderer->RemoveDrawable(drawable);
    //physics->RemoveHitBox(collisionBox);
}

void Cube::Hide()
{
    drawable->visible = false;
}

void Cube::Show()
{
    drawable->visible = true;
}

bool Cube::isVisible()
{
    return drawable->visible;
}

void Cube::Update()
{
    uniforms.Lock();
    drawable->uniformData = uniforms;
    drawable->matrix = matrix.subMatrix * matrix.matrix;
    pos = glm::vec3(drawable->matrix[3].x, drawable->matrix[3].y, drawable->matrix[3].z);
    collisionBox->position = pos;
    renderer->Draw(drawable);
}

void Cube::UpdateAfterPhysics()
{
}
