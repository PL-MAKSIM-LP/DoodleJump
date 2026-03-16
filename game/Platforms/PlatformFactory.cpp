#include "PlatformFactory.h"

#include <iostream>
#include <string>

#include "BreakablePlatform.h"
#include "MovingPlatformHorizontal.h"
#include "MovingPlatformVertical.h"
#include "NormalPlatform.h"
#include "core/Constants.h"

std::unique_ptr<IPlatform> PlatformFactory::create(int x, int y,
                                                   PlatformType type) {
  const char* path = "../sprites/normal.png";
  switch (type) {
    case PlatformType::NORMAL:
      path = "../sprites/normal.png";
      break;
    case PlatformType::MOVING_HORIZONTAL:
      path = "../sprites/moveHorizontal.png";
      break;
    case PlatformType::MOVING_VERTICAL:
      path = "../sprites/moveVertical.png";
      break;
    case PlatformType::BREAKABLE:
      path = "../sprites/break1.png";
      break;
  }

  Sprite* spr = createSprite(path);
  if (spr) setSpriteSize(spr, PlatformConfig::WIDTH, PlatformConfig::HEIGHT);

  switch (type) {
    case PlatformType::NORMAL: {
      auto p = std::make_unique<NormalPlatform>();
      p->init(x, y, spr, type);
      return p;
    }
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
      p->init(x, y, spr, type);
      return p;
    }
    default: {
      auto p = std::make_unique<NormalPlatform>();
      p->init(x, y, spr, PlatformType::NORMAL);
      return p;
    }
  }
}
