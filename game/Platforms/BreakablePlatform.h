#pragma once

#include <array>

#include "Platform.h"
#include "core/Constants.h"

class BreakablePlatform : public IPlatform {
 public:
  BreakablePlatform() = default;
  ~BreakablePlatform() override = default;

  void update(float dt, int screenWidth) override;
  void onCollide(Player& player) override;

  // Статический кэш спрайтов анимации (загружается один раз при инициализации
  // игры)
  static void initializeAnimationCache();
  static void clearAnimationCache();

 private:
  bool broken = false;
  bool falling = false;

  int breakStage = 0;  // 0 == intact, 1..N animation stages
  int maxBreakStages =
      BreakablePlatformConfig::MAX_BREAK_STAGES;  // number of breaking frames
  float breakTimer = 0.0f;
  float breakStepDuration =
      BreakablePlatformConfig::BREAK_STEP_DURATION;  // seconds per frame

  // Статический кэш спрайтов: break1.png, break2.png, break3.png, break4.png,
  // break5.png
  static std::array<Sprite*, 5> cachedBreakSprites;
  static bool cacheInitialized;
};
