#include "Projectile.h"

#include <algorithm>

void Projectile::update(float dt) {
  x += velX * dt;
  y += velY * dt;
  // No logging - update is called every frame for many projectiles
}

void Projectile::render(const Camera& camera, Sprite* sprite) {
  if (!active || !sprite) return;
  float screenY = camera.toScreenY(y);
  drawSprite(sprite, static_cast<int>(x), static_cast<int>(screenY));
  // No logging - render is called every frame for every projectile
}
