#pragma once

#include "Platform.h"

class BreakablePlatform : public IPlatform {
 public:
  BreakablePlatform() = default;
  ~BreakablePlatform() override = default;

  void update(float dt, int screenWidth) override;
  void onCollide(Player& player) override;

 private:
  bool broken = false;
  bool falling = false;

  int breakStage = 0;      // 0 == intact, 1..N animation stages
  int maxBreakStages = 4;  // number of breaking frames (e.g. break2..break5)
  float breakTimer = 0.0f;
  float breakStepDuration = 0.12f;  // seconds per frame
};
