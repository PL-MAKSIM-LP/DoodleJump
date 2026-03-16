#pragma once

#include "Platform.h"

class MovingPlatformHorizontal : public IPlatform {
 public:
  ~MovingPlatformHorizontal() override = default;
  void setMovement();
  void update(float dt, int screenWidth) override;

 private:
  float moveSpeed = 10.0f;
  float moveDir = 1.0f;
  float minX = 0.0f;
  float maxX = 0.0f;
};
