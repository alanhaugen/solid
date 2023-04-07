#include "invader.h"

Invader::Invader(Actor &invader)
    : invader_(invader) {
    isActive_ = true;

    /*Bullet = new Sprite(0,0,  OBJ_SIZE_32X32, bulletTiles, sizeof (bulletTiles));
    Bullet->hide();
    Bullet->vflip();*/

    speed_ = 1;
    dir_ = -1;
};

void Invader::Update() {
    if (isActive_) {
        sprite_->x += dir_ * speed_;

        // Fire a bullet at player randomly
        /*if (qran_range(0, 500) == 1) {
            Bullet->show();
            Bullet->x = x;
            Bullet->y = y;
        }*/
    }
    else {
        sprite_->Hide();
    }

    /*	if (Bullet->y > SCREEN_HEIGHT - 10) {
        Bullet->x = 0;
        Bullet->hide();
    }

    if (Bullet->x != 0) {
        Bullet->y++;
    }*/
};

Invader::~Invader() {
    //delete Bullet;
};
