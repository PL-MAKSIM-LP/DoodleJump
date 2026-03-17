#include "MovingPlatformVertical.h"

#include <cmath>
#include <iostream>
#include <random>

#include "core/Constants.h"

void MovingPlatformVertical::update(float dt, int screenHeight) {
  float oldY = y;
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

  static std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dist(-1.0f, 1.0f);  // TODO
  moveDir = dist(gen);
}
