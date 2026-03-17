#pragma once

#include <memory>
#include <random>
#include <set>
#include <vector>

#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "BonusFactory.h"
#include "IBonus.h"

class PlatformManager;
class IPlatform;

class BonusManager {
 public:
  BonusManager();
  ~BonusManager() = default;

  void update(int screenWidth, int screenHeight, const Camera& camera,
              const Player& player);
  void render(const Camera& camera);
  void clear();  // Очистить все бонусы

  bool spawnedTrampoline = false;

  void setPlatformManager(PlatformManager* pm) { platformManager = pm; }
  void setRandomGenerator(std::mt19937& generator) { rng = &generator; }

 private:
  std::vector<std::unique_ptr<IBonus>> coins;
  std::unique_ptr<IBonus> trampoline;

  BonusFactory bonusFactory;

  float lastSpawnY = 0.0f;
  bool firstSpawn = true;

  unsigned int timeSinceLastTrampoline = 0;

  void spawnCoin(int x, int y);
  void spawnTrampoline(int x, int y);
  void spawnCoinsOnPlatforms();

  int coinCapacity = 10;
  PlatformManager* platformManager = nullptr;
  std::mt19937* rng = nullptr;
  std::set<const IPlatform*>
      platformsWithCoins;  // Запомни платформы которые уже получили монетки
};
