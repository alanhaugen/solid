#include "cube.h"

Cube::Cube(float x, float y, float z, float length, float width, float height, String shaderVertPath, String shaderFragPath)
{
    *matrix.x = x;
    *matrix.y = y;
    *matrix.z = z;

    matrix.position = glm::vec3(x,y,z);

    Array<Pixmap*> *textures = NULL;
    Array<String> shaders(2);

    IFile *vertexFile = filesystem->Open(URL(shaderVertPath), PLAIN_TEXT);
    IFile *fragmentFile = filesystem->Open(URL(shaderFragPath), PLAIN_TEXT);

    shaders[VERTEX_SHADER]   = vertexFile->Read();
    shaders[FRAGMENT_SHADER] = fragmentFile->Read();

    delete vertexFile;
    delete fragmentFile;

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

    // 36 coords for a cube
    // Thanks to Dr. Anton https://antongerdelan.net/opengl/cubemaps.html
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));

    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(1.0f*length, -1.0f*height, -1.0f*width)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length,  1.0f*height, -1.0f*width)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height, -1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-1.0f*length, -1.0f*height,  1.0f*width)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 1.0f*length, -1.0f*height,  1.0f*width)));

    tag = "cube";
    drawable = renderer->CreateDrawable(vertices, indices, shaders, textures);
    collisionBox = physics->CreateHitBox(glm::vec3(0.5f), &matrix, tag);
    drawable->matrix = matrix.matrix;
}

Cube::~Cube()
{
    renderer->RemoveDrawable(drawable);
    physics->RemoveHitBox(collisionBox);
}

void Cube::Update()
{
    drawable->uniformData = uniforms;
    drawable->matrix = matrix.subMatrix * matrix.matrix;
    pos = glm::vec3(drawable->matrix[3].x, drawable->matrix[3].y, drawable->matrix[3].z);
    *collisionBox->position = pos;
    renderer->Draw(drawable);
}

void Cube::UpdateAfterPhysics()
{
}
