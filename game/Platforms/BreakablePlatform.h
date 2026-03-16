#pragma once

#include "Platform.h"
#include "core/Constants.h"

class BreakablePlatform : public IPlatform {
 public:
  BreakablePlatform() = default;
  ~BreakablePlatform() override = default;

  void update(float dt, int screenWidth) override;
  void onCollide(Player& player) override;

 private:
  bool broken = false;
  bool falling = false;

  int breakStage = 0;  // 0 == intact, 1..N animation stages
  int maxBreakStages =
      BreakablePlatformConfig::MAX_BREAK_STAGES;  // number of breaking frames
  float breakTimer = 0.0f;
  float breakStepDuration =
      BreakablePlatformConfig::BREAK_STEP_DURATION;  // seconds per frame
};
