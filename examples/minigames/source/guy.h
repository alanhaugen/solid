#ifndef GUY_H
#define GUY_H

#include <core/components/component.h>
#include <core/components/sprite.h>
#include <core/x-platform/actor.h>
#include <cstddef>

class GuyState : public IComponent {
public:
    GuyState(Actor &guy);

    void Update();

private:
    Actor &guy_;

    Sprite *sprite_;

    bool isActive_;
	
    float speed_;
    float force_;
	
    int saved_;
    int lost_;
	
    int difficulty_;
	
    int startPosX_;
    int startPosY_;
};

#endif
