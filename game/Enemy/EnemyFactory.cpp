#include "EnemyFactory.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../framework/Framework.h"
#include "FlyingEnemy.h"
#include "GroundedEnemy.h"
#include "core/Constants.h"

EnemyFactory::EnemyFactory()
    : groundSprite(nullptr), flyingFramesInitialized(false) {}

EnemyFactory::~EnemyFactory() {
  std::cerr << "[EnemyFactory::~EnemyFactory] start" << std::endl;

  if (groundSprite) {
    destroySprite(groundSprite);
    groundSprite = nullptr;
  }

  for (auto f : flyingFrames) {
    if (f) {
      destroySprite(f);
    }
  }
  flyingFrames.clear();

  std::cerr << "[EnemyFactory::~EnemyFactory] end" << std::endl;
}

std::unique_ptr<IEnemy> EnemyFactory::create(int x, int y, EnemyType type) {
  std::cerr << "[EnemyFactory::create] Enemy: " << static_cast<int>(type)
            << std::endl;

  switch (type) {
    case EnemyType::FLYING: {
      if (!flyingFramesInitialized) {
        for (int i = 1; i <= EnemyConfig::FLY_FRAMES_COUNT; ++i) {
          std::string path = std::string(EnemyConfig::SPRITE_FLYING_BASE_PATH) +
                             std::to_string(i) + ".png";

          Sprite* s = createSprite(path.c_str());
          if (s) {
            setSpriteSize(s, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
          }
          flyingFrames.push_back(s);
        }
        flyingFramesInitialized = true;
      }

      auto enemy = std::make_unique<FlyingEnemy>();
      enemy->init(x, y, flyingFrames.empty() ? nullptr : flyingFrames.front(),
                  type);
      if (!flyingFrames.empty()) {
        enemy->setFlyFrames(flyingFrames, 0.12f);
      }
      return enemy;
    }
    case EnemyType::GROUND: {
      if (!groundSprite) {
        groundSprite = createSprite(EnemyConfig::SPRITE_GROUNDED_PATH);
        if (groundSprite) {
          setSpriteSize(groundSprite, EnemyConfig::GROUND_W,
                        EnemyConfig::GROUND_H);
        }
      }

      auto enemy = std::make_unique<GroundedEnemy>();
      enemy->init(x, y, groundSprite, type);
      return enemy;
    }
    default:
      return nullptr;
  }
}
