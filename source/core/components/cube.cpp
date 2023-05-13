#include "cube.h"

Cube::Cube(float x, float y, float z, float length, float width, float height)
{
    *matrix.x = x;
    *matrix.y = y;
    *matrix.z = z;

    Array<Pixmap*> *textures = NULL;
    Array<String> shaders(2);
    shaders[VERTEX_SHADER]   = filesystem->Open(URL("data/phong.vert"), PLAIN_TEXT)->Read();
    shaders[FRAGMENT_SHADER] = filesystem->Open(URL("data/phong.frag"), PLAIN_TEXT)->Read();

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

    drawable = renderer->CreateDrawable(vertices, indices, shaders, textures);
    drawable->matrix = matrix.matrix;
}

Cube::~Cube()
{
    // TODO: delete cube from renderer
}

void Cube::Update()
{
    drawable->uniformData = uniforms;
    drawable->matrix = matrix.subMatrix * matrix.matrix;
    renderer->Draw(drawable);
}

void Cube::UpdateAfterPhysics()
{
}
