#include "Platform.h"

void Platform::update(float dt, int screenWidth) {
    if (!active) return;

    if (type == PlatformType::MOVING) {
        x += moveDir * moveSpeed * dt;
        if (x <= 0 || x + w >= screenWidth)
            moveDir = -moveDir;
    }
}

void Platform::render(const Camera& camera, Sprite* sprite) {
    if (!active) return;
    drawSprite(sprite, (int)x, (int)camera.toScreenY(y));
}
