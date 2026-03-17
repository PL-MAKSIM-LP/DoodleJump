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
  if (groundSprite) destroySprite(groundSprite);
  for (auto f : flyingFrames) {
    if (f) destroySprite(f);
  }
  // Don't destroy groundedWalkFrames - they reference groundSprite which is
  // already destroyed
}

std::unique_ptr<IEnemy> EnemyFactory::create(int x, int y, EnemyType type) {
  // lazily create and cache sprites when first needed (framework must be
  // initialized before this is called)
  if (type == EnemyType::FLYING) {
    if (!flyingFramesInitialized) {
      for (int i = 1; i <= EnemyConfig::FLY_FRAMES_COUNT; ++i) {
        std::string path = std::string(EnemyConfig::SPRITE_FLYING_BASE_PATH) +
                          std::to_string(i) + ".png";
        Sprite* s = createSprite(path.c_str());
        if (s) {
          setSpriteSize(s, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
        } else {
          std::cerr << "⚠️  EnemyFactory: Failed to load " << path
                    << std::endl;
        }
        flyingFrames.push_back(s);
      }

      bool hasAny = false;
      for (auto f : flyingFrames)
        if (f) hasAny = true;
      if (!hasAny) {
        Sprite* s = createSprite(EnemyConfig::SPRITE_FLYING_FALLBACK);
        if (s) {
          setSpriteSize(s, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
          flyingFrames.assign(EnemyConfig::FLY_FRAMES_COUNT, s);
        } else {
          std::cerr << "❌ EnemyFactory: Failed to load fallback flying sprite"
                    << std::endl;
        }
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
  } else {
    if (!groundSprite) {
      groundSprite = createSprite(EnemyConfig::SPRITE_GROUNDED_PATH);
      if (groundSprite) {
        setSpriteSize(groundSprite, EnemyConfig::GROUND_W,
                      EnemyConfig::GROUND_H);
      } else {
        std::cerr << "❌ EnemyFactory: Failed to load grounded sprite"
                  << std::endl;
      }
    }

    if (!groundedWalkFramesInitialized) {
      initializeGroundedWalkFrames();
    }

    auto enemy = std::make_unique<GroundedEnemy>();
    enemy->init(x, y, groundSprite, type);

    if (!groundedWalkFrames.empty()) {
      enemy->setWalkFrames(groundedWalkFrames, 0.15f);
    }

    return enemy;
  }
}

void EnemyFactory::initializeGroundedWalkFrames() {
  // For now, use the same sprite 3 times
  // TODO: In the future, load separate walk animation frames
  // groundedWalkFrames = {walking_frame_1, walking_frame_2, walking_frame_3}
  if (groundSprite) {
    groundedWalkFrames.assign(EnemyConfig::GROUNDED_WALK_FRAMES_COUNT,
                              groundSprite);
  }
  groundedWalkFramesInitialized = true;
}
