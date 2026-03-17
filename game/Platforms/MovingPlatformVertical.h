#pragma once

#include "Platform.h"

class MovingPlatformVertical : public IPlatform {
 public:
  MovingPlatformVertical() = default;
  ~MovingPlatformVertical() override = default;
  void setMovement();
  void update(float dt, int screenHeight) override;

 private:
  float moveDir = 1.0f;
  float minY = 0.0f;
  float maxY = 0.0f;
};
