#pragma once

#include "Enemy.h"

class EnemyFactory {
 public:
  EnemyFactory() = default;
  ~EnemyFactory() = default;

  // Create a basic stationary enemy at (x,y)
  Enemy create(int x, int y) {
    Enemy e;
    e.x = static_cast<float>(x);
    e.y = static_cast<float>(y);
    return e;
  }
};
