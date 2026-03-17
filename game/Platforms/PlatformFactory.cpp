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
  std::cerr << "[PlatformFactory] Creating platform of type "
            << static_cast<int>(type) << " at x=" << x << " y=" << y
            << std::endl;
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
      p->setFactory(const_cast<PlatformFactory*>(this));
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
  // TODO
  std::cerr << "[PlatformFactory::PlatformFactory]" << std::endl;
  normalSprite = loadAndSetupSprite(PlatformConfig::SPRITE_NORMAL_PATH);
  movingHorizontalSprite =
      loadAndSetupSprite(PlatformConfig::SPRITE_MOVING_H_PATH);
  movingVerticalSprite =
      loadAndSetupSprite(PlatformConfig::SPRITE_MOVING_V_PATH);
  breakableSprite = loadAndSetupSprite(PlatformConfig::SPRITE_BREAKABLE_PATH);

  initializeBreakAnimationSprites();
}

PlatformFactory::~PlatformFactory() {
  std::cerr << "[PlatformFactory::~PlatformFactory]" << std::endl;

  if (normalSprite) {
    destroySprite(normalSprite);
    normalSprite = nullptr;
  }

  if (movingHorizontalSprite) {
    destroySprite(movingHorizontalSprite);
    movingHorizontalSprite = nullptr;
  }

  if (movingVerticalSprite) {
    destroySprite(movingVerticalSprite);
    movingVerticalSprite = nullptr;
  }

  if (breakableSprite) {
    destroySprite(breakableSprite);
    breakableSprite = nullptr;
  }

  clearBreakAnimationSprites();
}

void PlatformFactory::initializeBreakAnimationSprites() {
  std::cerr << "[PlatformFactory::initializeBreakAnimationSprites]"
            << std::endl;
  for (int i = 0; i < BreakablePlatformConfig::MAX_BREAK_SPRITES; ++i) {
    breakAnimationSprites[i] =
        createSprite(BreakablePlatformConfig::SPRITE_BREAK_PATHS[i]);
    if (breakAnimationSprites[i]) {
      setSpriteSize(breakAnimationSprites[i], PlatformConfig::WIDTH,
                    PlatformConfig::HEIGHT);
    }
  }
}

void PlatformFactory::clearBreakAnimationSprites() {
  std::cerr << "[PlatformFactory::clearBreakAnimationSprites]" << std::endl;
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
