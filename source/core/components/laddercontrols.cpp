#include "laddercontrols.h"

LadderControls::LadderControls()
{
    actor = NULL;

    Init();
}

LadderControls::LadderControls(Actor *_actor)
{
    actor = _actor;

    Init();

    actor->matrix.Translate(glm::vec3(-20, -14, -30));
    actor->matrix.Rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));

    groundY = actor->matrix.matrix[3].y;
}

void LadderControls::Init()
{
    speed = 0.016f;
    turnSpeed = 0.003f;
    isFacingLeft = false;
    isJumping = false;
}

void LadderControls::Update()
{
    idle = dynamic_cast<Mesh*>(actor->FindComponent("idle"));
    walk = dynamic_cast<Mesh*>(actor->FindComponent("walk"));
    attack = dynamic_cast<Mesh*>(actor->FindComponent("attack"));

    attack->Hide();
    idle->Show();
    walk->Hide();

    if (input.Mouse.Up)
    {
        idle->Hide();
        walk->Hide();
        attack->Hide();

        if (input.Held(input.Key.Q))
        {
            attack->Show();
        }
        else
        {
            if (isJumping)
            {
                actor->matrix.Translate(glm::vec3(0, momentumY, 0));

                momentumY -= 0.10f;

                if (actor->matrix.matrix[3].y < groundY)
                {
                    isJumping = false;
                    actor->matrix.matrix[3].y = groundY;
                }
            }
            /*if (input.Held(input.Key.W))
            {
                actor->matrix.Translate(actor->forward * speed * deltaTime);

                walk->Show();
            }
            if (input.Held(input.Key.S))
            {
                actor->matrix.Translate(-actor->forward * speed * deltaTime);

                walk->Show();
            }*/
            if (input.Held(input.Key.SPACE) && isJumping == false)
            {
                isJumping = true;
                momentumY = 1.5f;

                //jump->Show();
            }
            if (input.Held(input.Key.A) ||
                input.Held(input.Key.LEFT))
            {
                if (isFacingLeft == false)
                {
                    actor->matrix.Rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
                    isFacingLeft = true;
                }

                actor->matrix.Translate(glm::vec3(-speed * deltaTime, 0, 0));

                walk->Show();
            }
            if (input.Held(input.Key.D) ||
                input.Held(input.Key.RIGHT))
            {
                if (isFacingLeft == true)
                {
                    actor->matrix.Rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
                    isFacingLeft = false;
                }

                actor->matrix.Translate(glm::vec3(speed * deltaTime, 0, 0));

                walk->Show();
            }
            if (input.Held(input.Key.D) == false &&
                input.Held(input.Key.A) == false &&
                //input.Held(input.Key.SPACE) == false &&
                input.Held(input.Key.RIGHT) == false &&
                input.Held(input.Key.LEFT) == false)
            {
                idle->Show();
            }
        }
    }
}

void LadderControls::UpdateAfterPhysics()
{
}
