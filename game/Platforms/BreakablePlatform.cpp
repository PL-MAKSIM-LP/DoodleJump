#include "BreakablePlatform.h"

void BreakablePlatform::update(float dt, int screenWidth) {
  if (broken) {
    // fall down slowly
    y += 200.0f * dt;
    if (y > screenWidth + 1000) active = false;
  }
}

void BreakablePlatform::onCollide(Player& player) {
  // give a small bounce then break
  player.y = top() - player.height;
  player.velY = -player.velY * 0.6f;
  broken = true;
}
