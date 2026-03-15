#include "PlatformFactory.h"

#include <iostream>
#include <string>

#include "BreakablePlatform.h"
#include "MovingPlatformHorizontal.h"
#include "MovingPlatformVertical.h"
#include "NormalPlatform.h"

std::unique_ptr<IPlatform> PlatformFactory::create(int x, int y,
                                                   PlatformType type) {
  const char* path = "../data/01-Breakout-Tiles.png";
  switch (type) {
    case PlatformType::NORMAL:
      path = "../data/01-Breakout-Tiles.png";
      break;
    case PlatformType::MOVING_HORIZONTAL:
      path = "../data/01-Breakout-Tiles.png";
      break;
    case PlatformType::MOVING_VERTICAL:
      path = "../data/01-Breakout-Tiles.png";
      break;
    case PlatformType::BREAKABLE:
      path = "../data/01-Breakout-Tiles.png";
      break;
  }

  Sprite* spr = createSprite(path);
  setSpriteSize(spr, 70, 18);

  std::cout << "_______" << std::endl;
  std::cout << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;

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
