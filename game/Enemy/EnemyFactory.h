#pragma once

#include <memory>

#include "IEnemy.h"

class EnemyFactory {
 public:
  EnemyFactory();
  ~EnemyFactory();

  std::unique_ptr<IEnemy> create(int x, int y,
                                 EnemyType type = EnemyType::GROUND);

 private:
  Sprite* groundSprite = nullptr;
  Sprite* flyingSprite = nullptr;
};
