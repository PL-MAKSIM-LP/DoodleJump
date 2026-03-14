#include "Projectile.h"

void Projectile::update(float dt) {
    if (!active) return;
    y += speed * dt;
}

void Projectile::render(const Camera& camera, Sprite* sprite) {
    if (!active) return;
    drawSprite(sprite, (int)x, (int)camera.toScreenY(y));
}
