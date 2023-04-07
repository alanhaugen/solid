#include "tankcontrols.h"

TankControls::TankControls()
{
    actor = NULL;

    Init();
}

TankControls::TankControls(Actor *_actor)
{
    actor = _actor;

    Init();
}

void TankControls::Init()
{
    speed = 0.01f;
    turnSpeed = 0.003f;
}

void TankControls::Update()
{
    idle = dynamic_cast<Mesh*>(actor->FindComponent("idle"));
    walk = dynamic_cast<Mesh*>(actor->FindComponent("walk"));
    attack = dynamic_cast<Mesh*>(actor->FindComponent("attack"));

//    attack->Hide();
//    idle->Show();
//    walk->Hide();

    if (input.Mouse.Up)
    {
//        idle->Hide();
//        walk->Hide();
//        attack->Hide();

        if (input.Held(input.Key.Q))
        {
            //attack->Show();
        }
        else
        {
            if (input.Held(input.Key.W))
            {
                actor->matrix.Translate(actor->forward * speed * deltaTime);

                //walk->Show();
            }
            if (input.Held(input.Key.S))
            {
                actor->matrix.Translate(-actor->forward * speed * deltaTime);

                //walk->Show();
            }
            if (input.Held(input.Key.A))
            {
                actor->matrix.Rotate(turnSpeed * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

                //walk->Show();
            }
            if (input.Held(input.Key.D))
            {
                actor->matrix.Rotate(-turnSpeed * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

                //walk->Show();
            }
            if (input.Held(input.Key.D) == false && input.Held(input.Key.A) == false &&
                    input.Held(input.Key.S) == false && input.Held(input.Key.W) == false)
            {
                //idle->Show();
            }
        }
    }
}

void TankControls::UpdateAfterPhysics()
{

}
