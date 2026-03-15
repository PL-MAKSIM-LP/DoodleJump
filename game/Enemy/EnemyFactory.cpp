#include "EnemyFactory.h"

#include <memory>

#include "../../framework/Framework.h"
#include "EnemyConfig.h"
#include "FlyingEnemy.h"
#include "GroundedEnemy.h"

EnemyFactory::EnemyFactory() : groundSprite(nullptr), flyingSprite(nullptr) {}

EnemyFactory::~EnemyFactory() {
  if (groundSprite) destroySprite(groundSprite);
  if (flyingSprite) destroySprite(flyingSprite);
}

std::unique_ptr<IEnemy> EnemyFactory::create(int x, int y, EnemyType type) {
  // lazily create and cache sprites when first needed (framework must be
  // initialized before this is called)
  if (type == EnemyType::FLYING) {
    if (!flyingSprite) {
      flyingSprite = createSprite("../sprites/enemyFlying.png");
      if (flyingSprite)
        setSpriteSize(flyingSprite, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
    }
    auto enemy = std::make_unique<FlyingEnemy>();
    enemy->init(x, y, flyingSprite, type);
    return enemy;
  } else {
    if (!groundSprite) {
      groundSprite = createSprite("../sprites/enemyGrounded.png");
      if (groundSprite)
        setSpriteSize(groundSprite, EnemyConfig::GROUND_W,
                      EnemyConfig::GROUND_H);
    }
    auto enemy = std::make_unique<GroundedEnemy>();
    enemy->init(x, y, groundSprite, type);
    return enemy;
  }
}
