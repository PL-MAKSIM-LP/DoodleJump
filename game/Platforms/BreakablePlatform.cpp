#include "BreakablePlatform.h"

#include <iostream>

#include "PlatformFactory.h"
#include "core/Constants.h"

void BreakablePlatform::update(float dt, int screenHeight) {
  if (broken && !falling) {
    breakTimer += dt;
    if (breakTimer >= breakStepDuration) {
      breakTimer = 0.0f;
      ++frame;

      if (frame <= maxBreakStages &&
          frame < BreakablePlatformConfig::MAX_BREAK_SPRITES) {
        if (factory) {
          sprite = factory->getBreakSprite(frame);
          if (sprite) setSpriteSize(sprite, w, h);
        } else {
          std::cerr << "[BreakablePlatform::update] factory is nullptr! "
                    << "breakStage=" << frame << std::endl;
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
    std::cerr << "[BreakablePlatform::onCollide] Breaking platform"
              << std::endl;
    broken = true;
    player.jump();
  }

  frame = 0;
  breakTimer = 0.0f;
  player.jump();
}
