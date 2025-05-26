#include <core/application.h>
#include <core/components/cube.h>
#include <core/components/camera.h>

class Primitives : public IScene
{
private:
    Cube *red;
    Cube *green;
    Cube *blue;

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
    red = new Cube(-2.0f, 0.0f, -10.0f);
    green = new Cube(2.0f, 0.0f, -10.0f);
    blue = new Cube(0.0f, -2.5f, -10.0f);

    components.Add(new Camera());
    components.Add(red);
    components.Add(green);
    components.Add(blue);
}

void Primitives::Update(float dt)
{
    red->matrix.Rotate(0.01f * dt, glm::vec3(1.0f, 1.0f, 1.0f));
    green->matrix.Rotate(0.01f * dt, glm::vec3(1.0f, 1.0f, 1.0f));
    blue->matrix.Rotate(0.01f * dt, glm::vec3(1.0f, 1.0f, 1.0f));
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

