#include "GroundedEnemy.h"

void GroundedEnemy::update(float dt, int screenWidth) {
  // Обновляем анимацию по таймеру
  if (!walkFrames.empty()) {
    animationTimer += dt;
    if (animationTimer >= frameDuration) {
      animationTimer = 0.0f;
      currentFrameIndex = (currentFrameIndex + 1) % walkFrames.size();

      // Переключаем спрайт
      sprite = walkFrames[currentFrameIndex];
    }
  }

  // Движение врага влево-вправо
  x += velocityX;

  // Проверка границ и разворот
  if (x <= 0 || x >= screenWidth - w) {
    velocityX *= -1;
  }
}