#pragma once

#include "Platform.h"

class PlatformFactory;

class BreakablePlatform : public IPlatform {
 public:
  BreakablePlatform() = default;
  ~BreakablePlatform() override = default;

  void update(float dt, int screenHeight) override;
  void onCollide(Player& player) override;

  void setFactory(PlatformFactory* factory) { this->factory = factory; }

 private:
  bool broken = false;
  bool falling = false;

  int frame = 0;
  int maxBreakStages = BreakablePlatformConfig::MAX_BREAK_STAGES;
  float breakTimer = 0.0f;
  float breakStepDuration = BreakablePlatformConfig::BREAK_STEP_DURATION;

  PlatformFactory* factory = nullptr;
};
