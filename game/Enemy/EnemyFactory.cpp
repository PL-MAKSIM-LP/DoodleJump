#include "EnemyFactory.h"

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
}

std::unique_ptr<IEnemy> EnemyFactory::create(int x, int y, EnemyType type) {
  // lazily create and cache sprites when first needed (framework must be
  // initialized before this is called)
  if (type == EnemyType::FLYING) {
    if (!flyingFramesInitialized) {
      for (int i = 1; i <= 3; ++i) {
        std::string path = "../sprites/flying" + std::to_string(i) + ".png";
        Sprite* s = createSprite(path.c_str());
        if (s) setSpriteSize(s, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
        flyingFrames.push_back(s);
      }

      bool hasAny = false;
      for (auto f : flyingFrames)
        if (f) hasAny = true;
      if (!hasAny) {
        Sprite* s = createSprite("../sprites/enemyFlying.png");
        if (s) setSpriteSize(s, EnemyConfig::FLY_W, EnemyConfig::FLY_H);
        flyingFrames = {s, s, s};
      }

      flyingFramesInitialized = true;
    }

    auto enemy = std::make_unique<FlyingEnemy>();
    enemy->init(x, y, flyingFrames.front(), type);
    enemy->setFlyFrames(flyingFrames, 0.12f);
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

    // Пример анимации: используем один спрайт несколько раз (для теста)
    // В реальной игре загружали бы разные спрайты: enemyWalk1.png,
    // enemyWalk2.png
    std::vector<Sprite*> walkFrames = {groundSprite, groundSprite,
                                       groundSprite};
    enemy->setWalkFrames(walkFrames, 0.15f);  // 3 кадра, 150ms между ними

    return enemy;
  }
}
