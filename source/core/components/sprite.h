#ifndef SPRITE_H
#define SPRITE_H

#include "component.h"
#include "core/x-platform/url.h"
#include "modules/renderer/drawable.h"

class Pixmap;
class Sprite : public Component
{
protected:
    IDrawable *drawable;

private:
    ITime *timer;

    void Init(const float x_ = 0.0,
              const float y_ = 0.0,
              const float scaleX_ = 1.0f,
              const float scaleY_ = 1.0f,
              const int textureWidth_ = 0,
              const int textureHeight_ = 0,
              const unsigned int quadQuantity = 1,
              const char *glyphs = "");

    ITexture *texture;

public:
    Sprite(String textureFilePath,
           const float x_ = 0.0,
           const float y_ = 0.0,
           const float scaleX_ = 1.0f,
           const float scaleY_ = 1.0f,
           const int textureWidth_ = 0,
           const int textureHeight_ = 0,
           const unsigned int quadQuantity = 1,
           const char *glyphs = "");
    Sprite(const int red,
           const int green,
           const int blue,
           const float x_ = 0.0,
           const float y_ = 0.0,
           const float scaleX_ = 1.0f,
           const float scaleY_ = 1.0f,
           const int textureWidth_ = 0,
           const int textureHeight_ = 0,
           const unsigned int quadQuantity = 1,
           const char *glyphs = "");
    Sprite(Pixmap *texture, const float _x = 0.0, const float _y = 0.0, const int _textureWidth = 0, const int _textureHeight = 0);
    ~Sprite();

    void Update();
    void UpdateAfterPhysics();

    // Add animation
    void AddAnimation(int anim, int frames[], int frames_length, int framerate, bool looping);

    // Play
    void PlayAnimation(int anim, bool reset = false);

    // Flip sprite
    void FlipHorizontal();
    void FlipVertical();

    // Hide and unhide
    void Show();
    void Hide();

    // Rotation
    void Rotate(int radians);

    // Scale a certain amount in x, y or both
    void Scale(const float scale, bool x = true, bool y = true);

    float halfWidth, halfHeight;
    float scaleX, scaleY;
    int width, height;
    bool isVisible;
    int index;
    float x,y;
    bool isFlipped;
    bool isFlippedVertical;
};

#endif // SPRITE_H
