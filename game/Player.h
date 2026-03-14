#pragma once

#include "../framework/Framework.h"
#include "Camera.h"

class Player {
public:
    float x = 0.0f;
    float y = 0.0f;
    float velX = 0.0f;
    float velY = 0.0f;
    int w = 50;
    int h = 60;

    bool alive = true;

    Player();
    ~Player();

    void init(int screenWidth, int screenHeight);
    void update(float dt, int screenWidth);
    void render(const Camera& camera);

    void jump();
    void onKeyPressed(FRKey k);
    void onKeyReleased(FRKey k);

    // AABB bounds in world space
    float left()   const { return x; }
    float right()  const { return x + w; }
    float top()    const { return y; }
    float bottom() const { return y + h; }

private:
    Sprite* spriteLeft  = nullptr;
    Sprite* spriteRight = nullptr;

    bool movingLeft  = false;
    bool movingRight = false;
};
