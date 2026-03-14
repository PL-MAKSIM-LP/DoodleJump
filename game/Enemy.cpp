#include "Enemy.h"

void Enemy::update(float dt, int screenWidth) {
    if (!alive) return;

    x += moveDir * moveSpeed * dt;
    if (x <= 0 || x + w >= screenWidth)
        moveDir = -moveDir;
}

void Enemy::render(const Camera& camera, Sprite* sprite) {
    if (!alive) return;
    drawSprite(sprite, (int)x, (int)camera.toScreenY(y));
}
