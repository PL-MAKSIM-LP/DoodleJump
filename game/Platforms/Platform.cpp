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
  return !(player.x + player.width <= left() || player.x >= right() ||
           player.y + player.height <= top() || player.y >= bottom()) &&
         player.velY > 0.0f;
}

void IPlatform::reset(int x, int y) {
  this->x = x;
  this->y = y;
  this->active = true;
}
