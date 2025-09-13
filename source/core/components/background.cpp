#include <core/application.h>
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

    // A cube which covers the entire screen
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

    shaders.Insert("data/bg.vert", VERTEX_SHADER);
    shaders.Insert("data/bg.frag", FRAGMENT_SHADER);

    drawable = renderer->CreateDrawable(vertices, indices, shaders);
    //drawable->hasDepth    = false; // Don't render as 3D
    //drawable->sendToFront = false; // Render behind everything else

    matrix.Scale(glm::vec3(10.0f));
    drawable->uniforms.colour = glm::vec4(colour, 1.0f);
}

void Background::Init()
{
    Array<String> shaders(2);
    shaders[VERTEX_SHADER]   = "data/cubemap.vert";
    shaders[FRAGMENT_SHADER] = "data/cubemap.frag";

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
    drawable->hasDepth    = false; // Don't render as 3D
    drawable->sendToFront = false; // Render behind everything else
}

Background::Background(String texturePath, Camera *camera)
{
    texture = renderer->CreateTexture(texturePath);

    scrolling = false;
    activeCamera = camera;

    Array<String> shaders(2);
    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;

    // A cube which covers the entire screen
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

    shaders.Insert("data/bg.vert", VERTEX_SHADER);
    shaders.Insert("data/bg.frag", FRAGMENT_SHADER);

    drawable = renderer->CreateDrawable(vertices, indices, shaders, texture);
    drawable->hasDepth    = false; // Don't render as 3D
    drawable->sendToFront = false; // Render behind everything else

    matrix.Scale(glm::vec3(10.0f));
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
        //Uniform("scrollX", static_cast<float>(scrollX += scrollSpeed));
        //Uniform("scrollY", static_cast<float>(scrollY += scrollSpeed));
    }

    //drawable->uniforms.time[0] = Application::time->TimeSinceStarted();

    // Note: hacky solution. We setup the uniforms and submatrix here
    //drawable->uniformData = uniforms;
    //drawable->matrix = matrix.subMatrix * matrix.matrix;
    renderer->Draw(drawable);
}

void Background::UpdateAfterPhysics()
{
    if (activeCamera)
    {
        drawable->matrix[3] = glm::vec4(activeCamera->position, 1.0f);
        //drawable->matrix = matrix.subMatrix * matrix.matrix;
    }
}
