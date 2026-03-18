#pragma once

#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "../Camera/Camera.h"
#include "../Enemy/EnemyFactory.h"
#include "../Enemy/IEnemy.h"
#include "../Player/Player.h"

enum class EnemyCollision { NOTHING, STOMP, HIT };
struct Object {
  float x, y, w, h;
};

class EnemyManager {
 public:
  EnemyManager() = default;
  ~EnemyManager() = default;

  void init(int screenWidth, int screenHeight);

  void update(float dt, int screenWidth, const Camera& camera, float playerY);
  void render(const Camera& camera);
  void clear();

  virtual void onCollide(Player& player) { player.jump(); };

  void setPlatformManager(class PlatformManager* pm);

  void setRandomGenerator(std::mt19937& generator) {
    rng = &generator;
    std::cerr << "[EnemyManager::setRandomGenerator] set generator"
              << std::endl;
  }

  EnemyCollision checkPlayerCollision(const Player& player);
  bool checkProjectileCollision(const Object& projectile);

 private:
  std::vector<std::unique_ptr<IEnemy>> enemies;

  int countAliveEnemies() const;
  void updateEnemiesFromCamera(float dt, int screenWidth, const Camera& camera);
  void cleanupDeadEnemies(const Camera& camera);
  void updateSpawnLogic(float playerY);
  void spawnMissingEnemies(float playerY);
  void spawnInitialBatch(float playerY);

  bool checkCollision(const Object& obj1, const Object& obj2) const;
  void spawnEnemy(int x, float y);

  int screenWidth = 0;
  int screenHeight = 0;

  class PlatformManager* platformManager = nullptr;
  EnemyFactory enemyFactory;
  std::mt19937* rng = nullptr;

  float spawnStartY;
  float lastSpawnY;
};
