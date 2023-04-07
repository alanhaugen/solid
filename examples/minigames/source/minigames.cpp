#include "minigames.h"

enum { MINIGAME_4, MINIGAME_2, MINIGAME_1, MINIGAME_3,  MINIGAME_OVER, HIGHSCORES };

const int FIREMEN_MIDDLE = 30;
enum { FIREMAN_STANDING, FIREMAN_WALKING };
int highscore;

MiniGames::MiniGames()
{
    // Keep score
    highscore = 0;

    // Menu
    menu_bg.Add(new Sprite(URL("data/ae.bmp")));
    menu.Add(new Sprite(URL("data/menu.bmp")));

    // Building
    bg2.Add(new Sprite(URL("data/building.bmp")));

    int guy_anim_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
    const int GUY_ANIM_LENGTH = 27;

    Sprite *sprite;

    for(int i = 0; i < 4; i++)
    {
        sprite = new Sprite(URL("data/guy.bmp"), 0, 0, 64, 64);
        sprite->AddAnimation(0, guy_anim_array, GUY_ANIM_LENGTH, 4, true);
        guy[i].Add(sprite);
    }

    sprite = new Sprite(URL("data/firemen.bmp"), 50.0f, 90.0f);
    sprite->Scale(2.0f, true, false);
    firemen.Add(sprite);

    score = 0;
    tick = 0;
    babies_lost = 0;

    // maze game
    //rat = sceneManager.NewSprite(0,0, "data/rat.bmp");
    //cheese = sceneManager.NewSprite(0,0, "data/cheese.bmp");

    // breakout
    sprite = new Sprite(URL("data/abstract.bmp"));
    sprite->Scale(0.1f);
    bg3.Add(sprite);

    // setup sprites
    for (int i = 0; i < BLOCKS; i++)
    {
        sprite = new Sprite(URL("data/block.bmp"), i%5*32 + 35, i/5 * 16 + 20);
        blocks[i].Add(sprite);
        sprite->Scale(2.0f);
    }

    ball.Add(new Sprite(URL("data/ball.bmp"), 50, 100));
    sprite = new Sprite(URL("data/paddle.bmp"), 50, 140);
    paddle.Add(sprite);
    sprite->Scale(6.0f, true, false);

    dirx = 1;
    diry = -1;
    nr_blocks = BLOCKS;

    // Spaceinvaders
    bg0.Add(new Sprite(URL("data/grass.bmp")));
    bg1.Add(new Sprite(URL("data/stars.bmp")));

    // setup sprites
    for (int i = 0; i < INVADERS; i++)
    {
        invaders[i].Add(new Sprite(URL("data/invader.bmp"), i%5*32 + 5,i/5 * 16));
    }

    for (int i = 0; i < BULLETS; i++)
    {
        bullets[i].Add(new Sprite(URL("data/bullet.bmp"), 0, 160));
        //bullets[i]->type = PLAYER_BULLET; // used for collisions
    }

    spaceship.Add(new Sprite(URL("data/spaceship.bmp"), 50, 120));

    score = 0;
    tick = 0;
    nr_invaders = INVADERS;
    game_over = false;

    scroll = 0.0;

    // Set state
    state = MINIGAME_OVER;
}

void MiniGames::UpdateLate()
{
    // Physics stuff here
    //std::cout << input->keys[59];//Held("START");
}

void MiniGames::Update()
{
    // Start button cycles through minigames
    if (input.Released("START"))
    {
        state = MINIGAME_OVER;
    }

    switch (state)
    {
    /*case HIGHSCORES:
            scroll += 0.5;
            if (scroll > 95) scroll = -95;
            menu_bg->x = scroll - 100;
            menu_bg->y = scroll - 100;

            if (input->Mouse.Down) {
                state = MINIGAME_OVER;
            }

            break;*/

    case MINIGAME_3:
        cheese.x = input.Mouse.x;
        cheese.y = input.Mouse.y;

        if (input.Mouse.Down)
        {
            float angle = atan2(input.Mouse.y - rat.y, input.Mouse.x - rat.x) * 180 / 3.14519;
            rat.x += cos(angle);
            rat.y += sin(angle);
        }

        break;

    case MINIGAME_4:

        if ((nr_invaders == 0 || game_over) && tick > 100)
        {
            state = MINIGAME_OVER;
        }

        //BGScrollXY(BG1, int(scroll=scroll+0.5), 0);
        scroll += 0.5;
        if (scroll > 100) scroll = -100;
        bg1.x = scroll;

        // Stop all movement on game_over
        if (spaceship.x > 10 && (input.Held("LEFT") || (input.Mouse.Down && input.Mouse.x - SPACESHIP_MIDDLE < spaceship.x)))
        {
            spaceship.x -= 3;
        }

        if (spaceship.x < 200 && (input.Held("RIGHT") || (input.Mouse.Down && input.Mouse.x - SPACESHIP_MIDDLE > spaceship.x)))
        {
            spaceship.x += 3;
        }

        if (input.Pressed("FIRE"))
        {
            // fire!
            for (int i = 0; i < BULLETS; i++)
            {
                if (bullets[i].x == 0)
                {
                    bullets[i].x = spaceship.x;
                    bullets[i].y = spaceship.y;
                    //bullets[i].show();
                    break;
                }
            }
        }

        // move bullets
        for (int i = 0; i < BULLETS; i++)
        {
            // active
            if (bullets[i].x != 0)
            {
                bullets[i].y -= 5;
            }
            // inactive
            else
            {
                //bullets[i]->hide();
            }
            // out of range
            if (bullets[i].y < 2)
            {
                bullets[i].x = 0;
                //bullets[i]->hide();
            }
        }
        /*
        for (int i = 0; i < INVADERS; i++) {
            invaders[i].Update();

            // change direction based on time (per 70 ticks)
            if (tick % 70 == 0) {
                invaders[i].dir = -invaders[i].dir;
                invaders[i].sprite->y += 6;
            }

            // lose
            if (invaders[i].sprite.y_ > 100 && invaders[i].isActive == true) {
                game_over = true;
            }

            // check invaders for collisions against player bullets
            //if (invaders[i]->collide( PLAYER_BULLET, invaders[i]->x, invaders[i]->y ))
            //	OutputSimpleText(0,0,"Hit!"); // (never works :( )

            // check invaders for collisions against player bullets
            for (int j = 0; j < BULLETS; j++) {
                if ((bullets[j]->isVisible && invaders[i].sprite->isVisible) &&
                        (bullets[j]->x+16 > invaders[i].sprite->x && bullets[j]->x < invaders[i].sprite->x+30) &&
                        (bullets[j]->y    < invaders[i].sprite->y && bullets[j]->y > invaders[i].sprite->y-30))
                {
                    bullets[j]->x = 0;
                    bullets[j]->hide();
                    invaders[i].isActive = false;
                    nr_invaders--;

                    if (nr_invaders == 0) game_over = true;
                }
            }

            // check player for collisions from invader bullets
            if ((invaders[i]->Bullet->isVisible) &&
                    (invaders[i]->Bullet->x+30 > spaceship->x && invaders[i]->Bullet->x < spaceship->x+30) &&
                    (invaders[i]->Bullet->y+30 > spaceship->y) && (invaders[i]->Bullet->y < spaceship->y))
                {
                    game_over = true;
                }
        }
        tick++;
        break;

    case MINIGAME_2:
        // Minigame 2
        if (babies_lost > PEOPLE_LOSE_AMOUNT)
        {
            if (highscore < score) highscore = score;
            state = MINIGAME_OVER;
        }

        score = 0;
        babies_lost = 0;

        for (int i = 0; i < 4; i++) {
            guy[i].Update();
            if (guy[i].sprite->y > 90 && firemen->x-10 < guy[i].sprite->x && firemen->x+30 > guy[i].sprite->x && guy[i].sprite->y < 120) {
                guy[i].force = -3;
            }

            score += guy[i].saved;
            babies_lost += guy[i].lost;

            if (tick > 2000)
                guy[i].difficulty = 1;
        }

        // Update text string to be displayed
        //sprintf(text, "People remaining: %d\nScore: %d", PEOPLE_LOSE_AMOUNT - babies_lost, score);

        if (firemen->x > 50 && (input->Held("LEFT") || (input->Mouse.Down && input->Mouse.x-FIREMEN_MIDDLE < firemen->x)))
            firemen->x -= PLAYER_SPEED;

        else if (firemen->x < 180 && (input->Held("RIGHT") || (input->Mouse.Down && input->Mouse.x-FIREMEN_MIDDLE > firemen->x)))
            firemen->x += PLAYER_SPEED;

        if (tick > 50)
            guy[0].isActive = true;
        if (tick > 400)
            guy[1].isActive = true;
        if (tick > 1000)
            guy[2].isActive = true;
        if (tick > 1250)
            guy[3].isActive = true;

        tick++;
        break;

    case MINIGAME_1:
        // Minigame 1
        if (ball->y > 160)
        {
            state = MINIGAME_OVER;
        }
        //if (nr_blocks == 0) OutputSimpleText(10, 1, "You win!");

        // Check collisions against walls
        if (ball->y < 3) {
            diry = -diry;
        }
        else if (ball->x > windowLength - 20 || ball->x < 10) {
            dirx = -dirx;
        }
        else if (ball->y > 150) {
            //OutputSimpleText(10, 1, "Game over");
        }

        // Move ball
        ball->x+=dirx;
        ball->y+=diry;

        // Collision with paddle
        if (ball->y > paddle->y-10 && ball->x > paddle->x && ball->x < paddle->x+50 && ball->y < paddle->y-5) {
            diry = -diry;
            dirx = float(ball->x - (paddle->x+25))/8;
        }

        if (spaceship->x > 5 && (input->Held("LEFT") || (input->Mouse.Down && input->Mouse.x-PADDLE_MIDDLE < paddle->x)))
            paddle->x -= 2;

        else if (spaceship->x < 180 && (input->Held("RIGHT") || (input->Mouse.Down && input->Mouse.x-PADDLE_MIDDLE > paddle->x)))
            paddle->x += 2;

        for (int i = 0; i < BLOCKS; i++) {
            // check collisions against ball
            if ((blocks[i]->isVisible) &&
                    (ball->x+16 > blocks[i]->x && ball->x < blocks[i]->x+30) &&
                    (ball->y-16 < blocks[i]->y && ball->y > blocks[i]->y-30))
            {
                // top
                if (ball->y > blocks[i]->y-30) {
                    diry = -diry;
                }
                // left
                else if (ball->x+16 > blocks[i]->x) {
                    dirx = -dirx;
                }
                // right
                else if (ball->x < blocks[i]->x+30) {
                    dirx = -dirx;
                }
                // bottom
                else if (ball->y < blocks[i]->y) {
                    diry = -diry;
                }

                blocks[i]->hide();
                nr_blocks--;

                if (nr_blocks == 0) tick = 0;
            }
        }
        tick++;
        break;

    case MINIGAME_OVER:
        score= 0;
        tick = 0;
        game_over = false;
        //sprintf(text, ""); //text = "";

        // reset bgs
        bg0->hide();
        bg1->hide();
        bg2->hide();
        bg3->hide();

        // Space invaders
        nr_invaders = INVADERS;
        spaceship->hide();

        // reset array of spaceinvaders
        for (int i = 0; i < INVADERS; i++) {
            invaders[i].sprite->hide();
            invaders[i].sprite->x = float(i%5*32 + 5);
            invaders[i].sprite->y = i/5 * 16;
            invaders[i].isActive = true;
            invaders[i].speed = 1;
            invaders[i].dir = -1;
        }

        // reset array of bullets
        for (int i = 0; i < BULLETS; i++) {
            bullets[i]->hide();
            bullets[i]->x = 0;
            bullets[i]->y = 160;
        }

        // Babies
        firemen->hide();
        babies_lost = 0;

        // reset guys
        for (int i = 0; i < 4; i++) {
            guy[i].isActive = false;
            guy[i].lost = false;
            guy[i].sprite->hide();
            guy[i].sprite->x = guy[i].start_pos_x;
            guy[i].difficulty = 2;
            guy[i].saved = false;
            guy[i].sprite->play(0);
        }

        // maze game
        //rat->hide();
        //cheese->hide();

        // breakout
        dirx = 1;
        diry = -1;

        ball->hide();
        paddle->hide();

        // reset array of blocks
        for (int i = 0; i < BLOCKS; i++) {
            blocks[i]->hide();
        }

        // Choose minigame
        menu->show();
        menu_bg->show();
        scroll += 0.5;
        if (scroll > 95) scroll = -95;
        menu_bg->x = scroll - 100;
        menu_bg->y = scroll - 100;

        if (input->Mouse.Released) { // Held would be a better name than Down
            if ((input->Mouse.x>108 && input->Mouse.x<141) && (input->Mouse.y>69 && input->Mouse.y<109)) state = HIGHSCORES;

            if ((input->Mouse.x>8 && input->Mouse.x<115) && (input->Mouse.y>9 && input->Mouse.y<69)) state=MINIGAME_1;
            if ((input->Mouse.x>8 && input->Mouse.x<107) && (input->Mouse.y>69 && input->Mouse.y<88)) state=MINIGAME_1;

            if ((input->Mouse.x>131 && input->Mouse.x<246) && (input->Mouse.y>9 && input->Mouse.y<69)) state=MINIGAME_2;
            if ((input->Mouse.x>145 && input->Mouse.x<247) && (input->Mouse.y>69 && input->Mouse.y<88)) state=MINIGAME_2;

            if ((input->Mouse.x>8 && input->Mouse.x<107) && (input->Mouse.y>91 && input->Mouse.y<111)) state=MINIGAME_3;
            if ((input->Mouse.x>8 && input->Mouse.x<125) && (input->Mouse.y>111 && input->Mouse.y<169)) state=MINIGAME_3;

            if ((input->Mouse.x>142 && input->Mouse.x<247) && (input->Mouse.y>90 && input->Mouse.y<169)) state=MINIGAME_4;
            if ((input->Mouse.x>129 && input->Mouse.x<158) && (input->Mouse.y>111 && input->Mouse.y<169)) state=MINIGAME_4;
        }

        switch (state) {
        case HIGHSCORES:
            menu->hide();
            //sprintf(text, "\n\nHigh score screen!\nScore: %d\n\nPress ESC to exit", highscore);
            break;
        case MINIGAME_3:
            rat->show();
            cheese->show();
            bg1->show();
            menu->hide();
            menu_bg->hide();
            break;
        case MINIGAME_4:
            spaceship->show();
            bg0->show();
            bg1->show();

            // show spaceinvaders
            for (int i = 0; i < INVADERS; i++) {
                invaders[i].sprite->show();
            }
            break;
        case MINIGAME_2:
            firemen->show();
            bg2->show();

            break;
        case MINIGAME_1:
            ball->show();
            paddle->show();
            bg3->show();

            ball->x = 50;
            ball->y = 100;

            paddle->x = 50;

            // show blocks
            for (int i = 0; i < BLOCKS; i++) {
                blocks[i]->show();
            }
            */break;
    case MINIGAME_OVER:
        break;
    default:
        LogError("No such minigame");
    }
}
