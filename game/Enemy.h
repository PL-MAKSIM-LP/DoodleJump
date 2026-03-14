#pragma once

#include "../framework/Framework.h"
#include "Camera.h"

class Enemy {
public:
    float x = 0.0f;
    float y = 0.0f;
    int w = 50;
    int h = 40;

    bool alive = true;

    void update(float dt, int screenWidth);
    void render(const Camera& camera, Sprite* sprite);

    float left()   const { return x; }
    float right()  const { return x + w; }
    float top()    const { return y; }
    float bottom() const { return y + h; }

private:
    float moveDir   = 1.0f;
    float moveSpeed = 60.0f;
};
