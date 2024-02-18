#include "background.h"

Background::Background()
{
    activeCamera = NULL;

    Init();
}

Background::Background(Camera *camera)
{
    activeCamera = camera;

    Init();
}

Background::~Background()
{
}

Background::Background(glm::vec3 colour, Camera *camera)
{
    scrolling = false;
    activeCamera = camera;

    Array<String> shaders(2);
    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

    // Two triangles making a quad over the entire screen
    vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f, -1.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f, -1.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f,  1.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec2(-1.0f,  1.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f, -1.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec2( 1.0f,  1.0f)));

    vertices[0].textureCoordinates = glm::vec2(0, 0);
    vertices[1].textureCoordinates = glm::vec2(1, 0);
    vertices[2].textureCoordinates = glm::vec2(0, 1);
    vertices[3].textureCoordinates = glm::vec2(0, 1);
    vertices[4].textureCoordinates = glm::vec2(1, 0);
    vertices[5].textureCoordinates = glm::vec2(1, 1);

    indices.Add(0);
    indices.Add(1);
    indices.Add(2);
    indices.Add(3);
    indices.Add(4);
    indices.Add(5);

    IFile *simpleVertShader = filesystem->Open(URL("data/bg.vert"), PLAIN_TEXT);
    IFile *simpleFragShader = filesystem->Open(URL("data/bg.frag"), PLAIN_TEXT);

    shaders.Insert(simpleVertShader->Read(), VERTEX_SHADER);
    shaders.Insert(simpleFragShader->Read(), FRAGMENT_SHADER);

    delete simpleVertShader;
    delete simpleFragShader;

    drawable = renderer->CreateDrawable(vertices, indices, shaders);
    drawable->hasDepth = false;
}

void Background::Init()
{
    Array<String> shaders(2);
    shaders[VERTEX_SHADER]   = filesystem->Open(URL("data/cubemap.vert"), PLAIN_TEXT)->Read();
    shaders[FRAGMENT_SHADER] = filesystem->Open(URL("data/cubemap.frag"), PLAIN_TEXT)->Read();

    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

    // 36 coords for a cube
    // Thanks to Dr. Anton https://antongerdelan.net/opengl/cubemaps.html
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));

    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(10.0f, -10.0f, -10.0f)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f,  10.0f, -10.0f)));

    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f, -10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3(-10.0f, -10.0f,  10.0f)));
    vertices.Add(IDrawable::Vertex(glm::vec3( 10.0f, -10.0f,  10.0f)));

    drawable = renderer->CreateDrawable(vertices, indices, shaders, texture);
    drawable->hasDepth = false; // Don't render as 3D
}

Background::Background(String texturePath, Camera *camera)
{
    texture = renderer->CreateTexture(texturePath);

    scrolling = false;
    activeCamera = camera;

    Init();
}

Background::Background(String texturePath, float scrollX_, float scrollY_, Camera *camera)
{
    scrollX = scrollX_;
    scrollY = scrollY_;

    scrollSpeed = (scrollX + scrollY) / 2;
    scrolling = true;

    texture = renderer->CreateTexture(texturePath);
    activeCamera = camera;

    Init();
}

Background::Background(String front,
                       String back,
                       String top,
                       String bottom,
                       String left,
                       String right,
                       Camera *camera)
{
    scrolling = false;
    activeCamera = camera;

    texture = renderer->CreateTexture(front, back, top, bottom, left, right);

    Init();
}

void Background::Update()
{
    if (scrolling)
    {
        Uniform("scrollX", static_cast<float>(scrollX += scrollSpeed));
        Uniform("scrollY", static_cast<float>(scrollY += scrollSpeed));
    }

    renderer->Draw(drawable);
}

void Background::UpdateAfterPhysics()
{
    if (activeCamera)
    {
        matrix.matrix[3] = glm::vec4(activeCamera->position, 1);
        drawable->matrix = matrix.subMatrix * matrix.matrix;
    }
}
