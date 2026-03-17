#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Platform.h"

class PlatformFactory {
 public:
  PlatformFactory();
  ~PlatformFactory();
  void initialize();

  std::unique_ptr<IPlatform> create(int x, int y, PlatformType type);

  Sprite* getBreakSprite(int stage) const {
    if (stage >= 0 && stage < BreakablePlatformConfig::MAX_BREAK_SPRITES) {
      return breakAnimationSprites[stage];
    }
    return nullptr;
  }

 private:
  Sprite* loadAndSetupSprite(const char* path);

  std::unique_ptr<IPlatform> createPlatform(int x, int y, PlatformType type,
                                            Sprite* sprite) const;

  void initializeBreakAnimationSprites();
  void clearBreakAnimationSprites();

  Sprite* normalSprite = nullptr;
  Sprite* movingHorizontalSprite = nullptr;
  Sprite* movingVerticalSprite = nullptr;
  Sprite* breakableSprite = nullptr;

  std::array<Sprite*, BreakablePlatformConfig::MAX_BREAK_SPRITES>
      breakAnimationSprites = {};
  bool initialized = false;
};
