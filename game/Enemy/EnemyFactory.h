#pragma once

#include <memory>
#include <vector>

#include "IEnemy.h"

class EnemyFactory {
 public:
  EnemyFactory();
  ~EnemyFactory();

  std::unique_ptr<IEnemy> create(int x, int y,
                                 EnemyType type = EnemyType::GROUND);

 private:
  Sprite* groundSprite = nullptr;
  std::vector<Sprite*> flyingFrames;
  bool flyingFramesInitialized = false;
};
