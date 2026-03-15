#pragma once

#include "Platform.h"

class MovingPlatformVertical : public IPlatform {
 public:
  MovingPlatformVertical() = default;
  ~MovingPlatformVertical() override = default;
  void setMovement();
  void update(float dt, int screenWidth) override;

  float moveSpeed = 10.0f;
  float moveDir = 1.0f;
  float minY = 0.0f;
  float maxY = 0.0f;
};
