#include "guy.h"

GuyState::GuyState(Actor &guy)
    : guy_(guy) {
    startPosX_ = 10;
    startPosY_ = 0;

    isActive_ = false;
    saved_ = 0;
    lost_ = 0;

    sprite_ = dynamic_cast<Sprite*>(guy_.FindComponent("Sprite"));

    if (sprite_ == NULL) {
        //LogError("Failed to convert IComponent to Sprite.");
    }

    difficulty_ = 2;
}

void GuyState::Update() {
    if (isActive_) {
        if (sprite_->x > 240 || sprite_->x < 0 || sprite_->y > 180) {
            if (sprite_->x != startPosX_) {
                if (sprite_->x > 240)
                    saved_++;
				else
                    lost_++;
			}

            speed_ = 2;//float(qran_range(2, 4) / difficulty);
            force_ = 0.2f;

            sprite_->x = startPosX_;
            sprite_->y = startPosY_;

            sprite_->Show();
            sprite_->Rotate(2);
		}

        force_ += 0.1f;
        sprite_->x += int (speed_);
        sprite_->y += int (force_);
	}
	else {
        sprite_->Hide();
	}
}
