#include "PlatformFactory.h"

#include <iostream>

#include "BreakablePlatform.h"
#include "MovingPlatformHorizontal.h"
#include "MovingPlatformVertical.h"
#include "NormalPlatform.h"
#include "core/Constants.h"

Sprite* PlatformFactory::loadAndSetupSprite(const char* path) {
  if (!path) return nullptr;

  Sprite* sprite = createSprite(path);
  if (sprite) {
    setSpriteSize(sprite, PlatformConfig::WIDTH, PlatformConfig::HEIGHT);
    return sprite;
  }
  return nullptr;
}

std::unique_ptr<IPlatform> PlatformFactory::createPlatform(
    int x, int y, PlatformType type, Sprite* sprite) const {
  Sprite* spr = sprite ? sprite : normalSprite;

  switch (type) {
    case PlatformType::MOVING_VERTICAL: {
      auto p = std::make_unique<MovingPlatformVertical>();
      p->init(x, y, spr, type);
      p->setMovement();
      return p;
    }

    case PlatformType::MOVING_HORIZONTAL: {
      auto p = std::make_unique<MovingPlatformHorizontal>();
      p->init(x, y, spr, type);
      p->setMovement();
      return p;
    }

    case PlatformType::BREAKABLE: {
      auto p = std::make_unique<BreakablePlatform>();
      p->setFactory(const_cast<PlatformFactory*>(this));  // Pass factory reference
      p->init(x, y, spr, type);
      return p;
    }

    case PlatformType::NORMAL:
    default: {
      auto p = std::make_unique<NormalPlatform>();
      p->init(x, y, spr, PlatformType::NORMAL);
      return p;
    }
  }
}

PlatformFactory::PlatformFactory() {
  normalSprite = loadAndSetupSprite(PlatformConfig::SPRITE_NORMAL_PATH);
  movingHorizontalSprite =
      loadAndSetupSprite(PlatformConfig::SPRITE_MOVING_H_PATH);
  movingVerticalSprite =
      loadAndSetupSprite(PlatformConfig::SPRITE_MOVING_V_PATH);
  breakableSprite = loadAndSetupSprite(PlatformConfig::SPRITE_BREAKABLE_PATH);

  // Initialize break animation sprites (loaded once, cached for reuse)
  initializeBreakAnimationSprites();
}

PlatformFactory::~PlatformFactory() {
  // Clean up break animation sprites
  clearBreakAnimationSprites();
}

void PlatformFactory::initializeBreakAnimationSprites() {
  for (int i = 0; i < BreakablePlatformConfig::MAX_BREAK_SPRITES; ++i) {
    breakAnimationSprites[i] = createSprite(
        BreakablePlatformConfig::SPRITE_BREAK_PATHS[i]);
    if (breakAnimationSprites[i]) {
      setSpriteSize(breakAnimationSprites[i], PlatformConfig::WIDTH,
                    PlatformConfig::HEIGHT);
    }
  }
}

void PlatformFactory::clearBreakAnimationSprites() {
  for (auto& sprite : breakAnimationSprites) {
    if (sprite) {
      destroySprite(sprite);
      sprite = nullptr;
    }
  }
}

std::unique_ptr<IPlatform> PlatformFactory::create(int x, int y,
                                                   PlatformType type) {
  Sprite* sprite = nullptr;
  switch (type) {
    case PlatformType::MOVING_VERTICAL:
      sprite = movingVerticalSprite;
      break;
    case PlatformType::MOVING_HORIZONTAL:
      sprite = movingHorizontalSprite;
      break;
    case PlatformType::BREAKABLE:
      sprite = breakableSprite;
      break;
    case PlatformType::NORMAL:
    default:
      sprite = normalSprite;
      break;
  }
  return createPlatform(x, y, type, sprite);
}
