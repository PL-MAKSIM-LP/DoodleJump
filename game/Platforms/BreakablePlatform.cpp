#include "BreakablePlatform.h"

#include <string>

#include "core/Constants.h"

void BreakablePlatform::update(float dt, int screenWidth) {
  if (broken && !falling) {
    // progress breaking animation
    breakTimer += dt;
    if (breakTimer >= breakStepDuration) {
      breakTimer = 0.0f;
      ++breakStage;

      // advance sprite to next break stage if available
      if (breakStage <= maxBreakStages) {
        // file naming: break2.png, break3.png ... (stage 1 -> break2)
        int fileIndex = breakStage + 1;
        std::string path =
            "../sprites/break" + std::to_string(fileIndex) + ".png";
        // replace sprite
        if (sprite) destroySprite(sprite);
        sprite = createSprite(path.c_str());
        if (sprite) setSpriteSize(sprite, w, h);
      } else {
        // animation finished -> start falling
        falling = true;
      }
    }
  }

  if (falling) {
    y += BreakablePlatformConfig::FALL_SPEED * dt;
    if (y > screenWidth + BreakablePlatformConfig::OFFSCREEN_PADDING)
      active = false;
  }
}

void BreakablePlatform::onCollide(Player& player) {
  if (!broken) {
    broken = true;
    breakStage = 0;
    breakTimer = 0.0f;
  }
}
