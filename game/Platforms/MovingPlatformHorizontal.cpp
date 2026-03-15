#include "MovingPlatformHorizontal.h"

#include <cmath>
#include <iostream>

void MovingPlatformHorizontal::update(float dt, int screenWidth) {
  x += moveDir * moveSpeed * dt;
  if (x <= minX) {
    x = minX;
    moveDir = 1.0f;
  } else if (x + w >= maxX) {
    x = maxX - w;
    moveDir = -1.0f;
  }
}

void MovingPlatformHorizontal::setMovement() {
  std::cout << x << " " << y << std::endl;
  minX = x - 50.0f;
  maxX = x + 50.0f;
}
