#ifndef MINIGAMES_H
#define MINIGAMES_H

#include <math.h>
#include <core/x-platform/scene.h>
#include <core/x-platform/actor.h>
#include <core/components/sprite.h>
#include "guy.h"
#include "invader.h"

//babies
const unsigned int PLAYER_SPEED = 6;
const unsigned int PEOPLE_LOSE_AMOUNT = 25;

//spaceinvaders
const unsigned int INVADERS = 20;
const unsigned int BULLETS = 5;
const unsigned int SPACESHIP_MIDDLE = 20;

//breakout
const unsigned int BLOCKS = 20;
const unsigned int PADDLE_MIDDLE = 30;

class MiniGames : public IScene
{
public:
    MiniGames();

    void Update();
    void UpdateLate();

    int state;
    char text[50];

    // Menu
    Actor menu;
    Actor menu_bg;

    // Babies
    int score;
    int tick;
    int babies_lost;
    char buffer[50];

    Actor bg0;
    Actor bg1;
    Actor bg2;
    Actor bg3;
    Actor firemen;

    // mazegame
    Actor rat;
    Actor cheese;

    // Space invaders
    Actor spaceship;

    Actor guy[4];

    Actor invaders[INVADERS];
    Actor bullets[BULLETS];

    int nr_invaders;
    bool game_over;

    float scroll;

    enum {
        ENEMY_BULLET,
        PLAYER_BULLET
    };

    // breakouk
    int dirx;
    int diry;
    int nr_blocks;

    Actor ball;
    Actor paddle;

    Actor blocks[BLOCKS];
};

#endif // MINIGAMES_H
