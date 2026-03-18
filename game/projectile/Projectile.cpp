#include "Projectile.h"

#include <algorithm>

Projectile::Projectile() { spawnTime = getTickCount(); }

void Projectile::update(float dt) {
  x += velX * dt;
  y += velY * dt;

  int width = 0, height = 0;
  getScreenSize(width, height);
  if (x < 0) {
    x = width;
  } else if (x > width) {
    x = 0;
  }

  if (spawnTime + ProjectileConfig::SHOOT_LIFETIME < getTickCount()) {
    active = false;
  }
}

void Projectile::render(const Camera& camera, Sprite* sprite) {
  if (!active || !sprite) return;
  float screenY = camera.toScreenY(y);
  drawSprite(sprite, static_cast<int>(x), static_cast<int>(screenY));
}
