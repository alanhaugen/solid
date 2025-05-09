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
    void Update(float dt);
    void UpdateAfterPhysics();
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

void Primitives::Update(float dt)
{
    cube->matrix.Rotate(0.01f * dt, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Primitives::UpdateAfterPhysics()
{
}

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new Primitives());

    return application.Exec();
}

