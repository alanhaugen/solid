#include "background.h"

Background::Background()
{
    textures = NULL;
    activeCamera = NULL;

    Init();
}

Background::Background(Camera *camera)
{
    textures = NULL;
    activeCamera = camera;

    Init();
}

Background::~Background()
{
    // TODO: Loop through textures and delete
    delete textures;
}

Background::Background(glm::vec3 colour, Camera *camera)
{
    textures = NULL;
    scrolling = false;
    activeCamera = camera;

    Array<String> shaders(2);
    Array<IDrawable::Vertex> vertices;
    Array<unsigned int> indices;
    Array<Pixmap*> textures = NULL;

    //textures.Add(new Pixmap(textureFilePath));

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

    drawable = renderer->CreateDrawable(vertices, indices, shaders, &textures);
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

    drawable = renderer->CreateDrawable(vertices, indices, shaders, textures);
    drawable->hasDepth = false; // Don't render as 3D
}

Background::Background(String texture, Camera *camera)
{
    textures = new Array<Pixmap*>;
    textures->Add(new Pixmap(texture));

    scrolling = false;
    activeCamera = camera;

    Init();
}

Background::Background(Pixmap texture, float scrollX_, float scrollY_, Camera *camera)
{
    scrollX = scrollX_;
    scrollY = scrollY_;

    textures = new Array<Pixmap*>;
    textures->Add(new Pixmap(texture));

    scrolling = true;

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
    textures = NULL;
    scrolling = false;
    activeCamera = camera;

    textures = new Array<Pixmap*>;
    textures->Add(new Pixmap(front));
    textures->Add(new Pixmap(back));
    textures->Add(new Pixmap(top));
    textures->Add(new Pixmap(bottom));
    textures->Add(new Pixmap(left));
    textures->Add(new Pixmap(right));

    Init();
}

void Background::Update()
{
    if (scrolling)
    {
        Uniform("scrollX", static_cast<float>(scrollX += scrollX));
        Uniform("scrollY", static_cast<float>(scrollY += scrollY));
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
