#include "MovingPlatformHorizontal.h"

#include <cmath>
#include <iostream>
#include <random>

#include "core/Constants.h"

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
  minX = x - MovingPlatformConfig::SWING;
  maxX = x + MovingPlatformConfig::SWING;
  static std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
  moveDir = dist(gen);
}
