#include "BreakablePlatform.h"

#include <string>

#include "core/Constants.h"

// Определение статических членов класса
std::array<Sprite*, 5> BreakablePlatform::cachedBreakSprites = {
    nullptr, nullptr, nullptr, nullptr, nullptr};
bool BreakablePlatform::cacheInitialized = false;

// Инициализация кэша спрайтов (вызывается один раз при старте игры)
void BreakablePlatform::initializeAnimationCache() {
  if (cacheInitialized) return;  // Уже инициализирован

  // Загружаем спрайты break1.png, break2.png, break3.png, break4.png,
  // break5.png
  for (int i = 0; i < 5; ++i) {
    int fileIndex = i + 1;  // 1-indexed
    std::string path = "../sprites/break" + std::to_string(fileIndex) + ".png";
    cachedBreakSprites[i] = createSprite(path.c_str());
    // TODO: setSpriteSize если нужно унифицировать размеры
  }

  cacheInitialized = true;
}

// Очистка кэша при завершении программы
void BreakablePlatform::clearAnimationCache() {
  for (auto sprite : cachedBreakSprites) {
    if (sprite) destroySprite(sprite);
  }
  cachedBreakSprites.fill(nullptr);
  cacheInitialized = false;
}

void BreakablePlatform::update(float dt, int screenWidth) {
  if (broken && !falling) {
    // progress breaking animation
    breakTimer += dt;
    if (breakTimer >= breakStepDuration) {
      breakTimer = 0.0f;
      ++breakStage;

      // advance sprite to next break stage from cache (БЕЗ createSprite!)
      if (breakStage <= maxBreakStages && breakStage < 5) {
        sprite = cachedBreakSprites[breakStage];
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
