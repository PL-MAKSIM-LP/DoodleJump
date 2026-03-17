#pragma once

#include "Platform.h"

class MovingPlatformHorizontal : public IPlatform {
 public:
  ~MovingPlatformHorizontal() override = default;
  void setMovement();
  void update(float dt, int screenHeight) override;

 private:
  float moveDir = 1.0f;
  float minX = 0.0f;
  float maxX = 0.0f;
};
