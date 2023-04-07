#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

class CollisionBox {
public:
    CollisionBox();

    //! Collision detection
    virtual bool Collide(int type, int x, int y);
};

#endif // COLLISIONBOX_H
