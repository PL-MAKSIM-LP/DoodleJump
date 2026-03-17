#include "FlyingEnemy.h"

#include <iostream>

#include "core/Constants.h"

void FlyingEnemy::update(float dt, int /*screenWidth*/) {
  if (flyFrames.empty()) return;

  animationTimer += dt;
  if (animationTimer >= frameDuration) {
    animationTimer = 0.0f;
    int prevFrameIndex = currentFrameIndex;
    currentFrameIndex =
        (currentFrameIndex + 1) % static_cast<int>(flyFrames.size());
    sprite = flyFrames[currentFrameIndex];
    // ✅ Check for nullptr before using sprite
    if (sprite) {
      setSpriteSize(sprite, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
      // Uncomment to trace frame changes (very verbose):
      // std::cerr << "  Frame: " << prevFrameIndex << " -> " << currentFrameIndex << std::endl;
    } else {
      std::cerr << "⚠️  FlyingEnemy: Sprite frame is nullptr at index "
                << currentFrameIndex << std::endl;
    }
  }
}