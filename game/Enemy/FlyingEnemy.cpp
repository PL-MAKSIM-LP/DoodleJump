#include "FlyingEnemy.h"

void FlyingEnemy::update(float dt, int /*screenWidth*/) {
  if (flyFrames.empty()) return;

  animationTimer += dt;
  if (animationTimer >= frameDuration) {
    animationTimer = 0.0f;
    currentFrameIndex =
        (currentFrameIndex + 1) % static_cast<int>(flyFrames.size());
    sprite = flyFrames[currentFrameIndex];
    if (sprite) setSpriteSize(sprite, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
  }
}