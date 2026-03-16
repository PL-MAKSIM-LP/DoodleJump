#pragma once

#include <memory>
#include <vector>

#include "../Camera.h"
#include "BonusFactory.h"
#include "IBonus.h"

class BonusManager {
 public:
  BonusManager() = default;
  ~BonusManager() = default;

  void update(int screenWidth, int screenHeight, const Camera& camera,
              float playerY);

 private:
  std::vector<std::unique_ptr<IBonus>> coins;
  std::unique_ptr<IBonus> trampoline;

  BonusFactory bonusFactory;

  float lastSpawnY = 0.0f;
  bool firstSpawn = true;

  void spawnCoin(int x, float y);
  void spawnTrampoline(int x, float y);

  int coinCapacity = 5;
};
