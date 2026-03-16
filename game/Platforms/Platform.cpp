#include "Platform.h"

#include <cstdlib>

void IPlatform::render(const Camera& camera) {
  if (active && this->sprite) {
    float screenY = camera.toScreenY(y);
    drawSprite(this->sprite, x, static_cast<int>(screenY));
  }
}

void IPlatform::init(int x, int y, Sprite* spr, PlatformType t) {
  this->x = x;
  this->y = y;
  sprite = spr;
  type = t;
  if (sprite) setSpriteSize(sprite, this->w, this->h);
}

bool IPlatform::isColliding(const Player& player) const {
  // Only collide when player is falling
  if (player.velY <= 0.0f) return false;

  float playerLeft = player.x;
  float playerRight = player.x + player.width;
  float playerBottom = player.y + player.height;

  bool horizOverlap = !(playerRight <= left() || playerLeft >= right());
  const float LAND_TOLERANCE = 5.0f;
  bool landed =
      (playerBottom > top()) && (playerBottom - LAND_TOLERANCE <= top());

  return horizOverlap && landed;
}

void IPlatform::reset(int x, int y) {
  this->x = x;
  this->y = y;
  this->active = true;
}
