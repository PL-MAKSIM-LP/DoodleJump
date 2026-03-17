#include "BreakablePlatform.h"

#include <iostream>

#include "PlatformFactory.h"
#include "core/Constants.h"

void BreakablePlatform::update(float dt, int screenHeight) {
  if (broken && !falling) {
    breakTimer += dt;
    if (breakTimer >= breakStepDuration) {
      breakTimer = 0.0f;
      ++breakStage;

      if (breakStage <= maxBreakStages &&
          breakStage < BreakablePlatformConfig::MAX_BREAK_SPRITES) {
        if (factory) {
          sprite = factory->getBreakSprite(breakStage);
          if (sprite) setSpriteSize(sprite, w, h);
        } else {
          // 🔴 CRITICAL: Factory not set - will crash on render!
          std::cerr << "❌ CRITICAL: BreakablePlatform::factory is nullptr! "
                    << "breakStage=" << breakStage << std::endl;
        }
      } else {
        falling = true;
      }
    }
  }

  if (falling) {
    y += BreakablePlatformConfig::FALL_SPEED * dt;
    if (y > screenHeight + BreakablePlatformConfig::OFFSCREEN_PADDING)
      active = false;
  }
}

void BreakablePlatform::onCollide(Player& player) {
  if (!broken) {
    std::cerr << "🔴 [BreakablePlatform::onCollide] Breaking platform at x="
              << x << " y=" << y << std::endl;
    broken = true;
    player.jump();
  }

  breakStage = 0;
  breakTimer = 0.0f;

  player.jump();
}
