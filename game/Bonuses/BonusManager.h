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
  BonusManager() = default;
  ~BonusManager() = default;

  void update(int screenWidth, int screenHeight, const Camera& camera,
              const Player& player);
  void render(const Camera& camera);
  void clear();

  bool spawnedTrampoline = false;

  void setRandomGenerator(std::mt19937& generator) { rng = &generator; }

 private:
  // Константы вынесены в BonusConfig namespace в Constants.h

  std::vector<std::unique_ptr<IBonus>> coins;
  std::unique_ptr<IBonus> trampoline;

  BonusFactory bonusFactory;

  float lastSpawnY = 0.0f;

  unsigned int timeSinceLastTrampoline = 0;

  void spawnCoin(int x, int y);
  void spawnTrampoline(int x, int y);
  void spawnCoinsOnScreen(int screenWidth, int screenHeight,
                          const Camera& camera);

  PlatformManager* platformManager = nullptr;
  std::mt19937* rng = nullptr;
};
