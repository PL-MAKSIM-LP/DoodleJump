#include "MovingPlatformVertical.h"

#include <cmath>

#include "core/Constants.h"

void MovingPlatformVertical::update(float dt, int screenWidth) {
  y += moveDir * MovingPlatformConfig::MOVE_SPEED * dt;
  if (y <= minY) {
    y = minY;
    moveDir = 1.0f;
  } else if (y + h >= maxY) {
    y = maxY - h;
    moveDir = -1.0f;
  }
}

void MovingPlatformVertical::setMovement() {
  minY = y - MovingPlatformConfig::SWING;
  maxY = y + MovingPlatformConfig::SWING;

  moveDir = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
}
