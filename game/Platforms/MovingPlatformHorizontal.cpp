#include "MovingPlatformHorizontal.h"

#include <cmath>
#include <iostream>
#include <random>

#include "core/Constants.h"

void MovingPlatformHorizontal::update(float dt, int screenHeight) {
  float oldX = x;
  x += moveDir * MovingPlatformConfig::MOVE_SPEED * dt;

  if (x <= minX) {
    x = minX;
    moveDir = 1.0f;
    // Uncomment for direction changes logging:
    // std::cerr << "🔀 [MovingPlatformHorizontal] Hit minX, reversed" <<
    // std::endl;
  } else if (x + w >= maxX) {
    x = maxX - w;
    moveDir = -1.0f;
    // Uncomment for direction changes logging:
    // std::cerr << "🔀 [MovingPlatformHorizontal] Hit maxX, reversed" <<
    // std::endl;
  }
}

void MovingPlatformHorizontal::setMovement() {
  minX = x - MovingPlatformConfig::SWING;
  maxX = x + MovingPlatformConfig::SWING;
  static std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
  moveDir = dist(gen);
  std::cerr << "🔀 [MovingPlatformHorizontal::setMovement] minX=" << minX
            << " maxX=" << maxX << " moveDir=" << moveDir << std::endl;
}
