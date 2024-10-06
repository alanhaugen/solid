#include <core/application.h>
#include <core/components/cube.h>
#include <core/components/camera.h>

class Primitives : public IScene
{
private:
    Cube *cube;

public:
    Primitives();

    void Init();
    void Update();
};

Primitives::Primitives()
{
}

void Primitives::Init()
{
    cube = new Cube(0.0f, 0.0f, -10.0f);

    components.Add(new Camera());
    components.Add(cube);
}

void Primitives::Update()
{
    cube->matrix.Rotate(0.1, glm::vec3(1.0f, 1.0f, 1.0f));
}

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new Primitives());

    return application.Exec();
}

