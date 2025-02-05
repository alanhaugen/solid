#ifndef SPRITE_H
#define SPRITE_H

#include "component.h"
#include "core/x-platform/url.h"
#include "modules/renderer/drawable.h"

#define Y_OFFSET 0

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
              const glm::vec2 anchorPoint = glm::vec2(0,0),
              const int textureWidth_ = 0,
              const int textureHeight_ = 0,
              const unsigned int quadQuantity_ = 1,
              const char *glyphs = "");

    ITexture *texture;

public:
    Sprite(String textureFilePath,
           const float x_ = 0.0,
           const float y_ = 0.0,
           const float scaleX_ = 1.0f,
           const float scaleY_ = 1.0f,
           const glm::vec2 anchorPoint_ = glm::vec2(0,0),
           const int textureWidth_ = 0,
           const int textureHeight_ = 0,
           const unsigned int quadQuantity_ = 1,
           const char *glyphs = "");
    Sprite(const int red,
           const int green,
           const int blue,
           const float x_ = 0.0,
           const float y_ = 0.0,
           const float scaleX_ = 1.0f,
           const float scaleY_ = 1.0f,
           const glm::vec2 anchorPoint_ = glm::vec2(0,0),
           const int textureWidth_ = 0,
           const int textureHeight_ = 0,
           const unsigned int quadQuantity_ = 1,
           const char *glyphs = "");
    ~Sprite();

    void Update();
    void UpdateAfterPhysics();

    // Add animation
    void AddAnimation(int anim, int frames[], int frames_length, int framerate, bool looping);

    // GUI
    bool IsPressed();
    bool IsHoveredOver();

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

    glm::vec2 anchorPoint;
    int quadQuantity;
    float halfWidth, halfHeight;
    float scaleX, scaleY;
    float originalScaleX, originalScaleY;
    int width, height;
    bool isVisible;
    int index;
    float x,y;
    float transformedX, transformedY;
    bool isFlipped;
    bool isFlippedVertical;
};

#endif // SPRITE_H
