#pragma once

#include <memory>
#include <vector>

#include "../Camera/Camera.h"
#include "BonusFactory.h"
#include "IBonus.h"

class BonusManager {
 public:
  BonusManager() = default;
  ~BonusManager() = default;

  void update(int screenWidth, int screenHeight, const Camera& camera,
              float playerY);
  void render(const Camera& camera);
  void clear();  // Очистить все бонусы

  bool spawnedTrampoline = false;

 private:
  std::vector<std::unique_ptr<IBonus>> coins;
  std::unique_ptr<IBonus> trampoline;

  BonusFactory bonusFactory;

  float lastSpawnY = 0.0f;
  bool firstSpawn = true;

  unsigned int timeSinceLastTrampoline = 0;

  void spawnCoin(int x, int y);
  void spawnTrampoline(int x, int y);

  int coinCapacity = 5;
};
