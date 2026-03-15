#include "Enemy.h"

#include <iostream>

void Enemy::update(float dt, int screenWidth) {
  // stationary enemy: no movement for now
}

void Enemy::render(const Camera& camera, Sprite* sprite) {
  if (!alive || !sprite) return;
  float screenY = camera.toScreenY(y);
  drawSprite(sprite, static_cast<int>(x), static_cast<int>(screenY));
}
