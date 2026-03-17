#pragma once

#include <memory>
#include <vector>

#include "IEnemy.h"

class EnemyFactory {
 public:
  EnemyFactory();
  ~EnemyFactory();

  std::unique_ptr<IEnemy> create(int x, int y, EnemyType type);

 private:
  std::vector<Sprite*> flyingFrames;
  bool flyingFramesInitialized = false;

  Sprite* groundSprite = nullptr;
  bool groundedWalkFramesInitialized = false;
};
