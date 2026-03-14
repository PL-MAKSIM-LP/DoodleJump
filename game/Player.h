#pragma once

#include "../framework/Framework.h"
#include "Camera.h"

class Player {
public:
    float x = 0.0f;
    float y = 0.0f;
    float velX = 0.0f;
    float velY = 0.0f;
    int widht = 50;
    int height = 60;

    bool alive = true;

    Player();
    ~Player();

    void init(int screenWidth, int screenHeight);
    void update(float dt, int screenWidth);
    void render(const Camera& camera);

    void jump();
    void onKeyPressed(FRKey k);
    void onKeyReleased(FRKey k);

    float left()   const { return x; }
    float right()  const { return x + widht; }
    float top()    const { return y; }
    float bottom() const { return y + height; }

private:
    Sprite* spriteLeft  = nullptr;
    Sprite* spriteRight = nullptr;

    bool movingLeft  = false;
    bool movingRight = false;
};
