#pragma once

#include "../framework/Framework.h"
#include "Camera.h"

enum class PlatformType {
    NORMAL,
    MOVING,
    BREAKABLE
};

class Platform {
public:
    float x = 0.0f;
    float y = 0.0f;
    int w = 70;
    int h = 18;

    PlatformType type = PlatformType::NORMAL;
    bool active = true;  // false = broken, skip collision and render

    void update(float dt, int screenWidth);
    void render(const Camera& camera, Sprite* sprite);

    float left()   const { return x; }
    float right()  const { return x + w; }
    float top()    const { return y; }
    float bottom() const { return y + h; }

private:
    float moveDir = 1.0f;
    float moveSpeed = 80.0f;
};
