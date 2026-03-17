#include "MovingPlatformVertical.h"

#include <cmath>
#include <iostream>

#include "core/Constants.h"

void MovingPlatformVertical::update(float dt, int screenHeight) {
  float oldY = y;
  y += moveDir * MovingPlatformConfig::MOVE_SPEED * dt;

  if (y <= minY) {
    y = minY;
    moveDir = 1.0f;
    // Uncomment for direction changes logging:
    // std::cerr << "🔀 [MovingPlatformVertical] Hit minY, reversed" <<
    // std::endl;
  } else if (y + h >= maxY) {
    y = maxY - h;
    moveDir = -1.0f;
    // Uncomment for direction changes logging:
    // std::cerr << "🔀 [MovingPlatformVertical] Hit maxY, reversed" <<
    // std::endl;
  }
}

void MovingPlatformVertical::setMovement() {
  minY = y - MovingPlatformConfig::SWING;
  maxY = y + MovingPlatformConfig::SWING;
  std::cerr << "🔀 [MovingPlatformVertical::setMovement] minY=" << minY
            << " maxY=" << maxY << std::endl;
}
