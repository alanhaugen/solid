#include "example1.h"

Example1::Example1()
{
    Array<String> shaders(2);

    shaders[VERTEX_SHADER]   = filesystem->Open(URL("data/example1.vert"), PLAIN_TEXT)->Read();
    shaders[FRAGMENT_SHADER] = filesystem->Open(URL("data/example1.frag"), PLAIN_TEXT)->Read();

    Array<IDrawable::Vertex> vertices(3);
    Array<unsigned int> indices(0);

    drawable = renderer->CreateDrawable(vertices, indices, shaders);
    int vbo_id = drawable->UploadAttribute(glm::vec4());
    drawable->shader->ConnectAttribute(1, vbo_id);

    input.Define("NEXT", KeyMap::SPACE);

    Log(shaders[FRAGMENT_SHADER]);
}

void Example1::Update()
{
    /*if (input.Released("NEXT"))
    {
        //NextScene();
    }*/
}

void Example1::UpdateLate()
{

}
