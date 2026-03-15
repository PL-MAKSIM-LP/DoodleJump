#include "MovingPlatformVertical.h"

#include <cmath>

void MovingPlatformVertical::update(float dt, int screenWidth) {
  y += moveDir * moveSpeed * dt;
  if (y <= minY) {
    y = minY;
    moveDir = 1.0f;
  } else if (y + h >= maxY) {
    y = maxY - h;
    moveDir = -1.0f;
  }
}

void MovingPlatformVertical::setMovement() {
  minY = y - 50.0f;
  maxY = y + 50.0f;
}
