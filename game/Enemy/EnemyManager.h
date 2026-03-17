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

class EnemyManager {
 public:
  EnemyManager() = default;
  ~EnemyManager() = default;

  void init(int screenWidth, int screenHeight);
  // playerY: current player world Y coordinate (used to decide when to spawn
  // enemies)
  void update(float dt, int screenWidth, const Camera& camera, float playerY);
  void render(const Camera& camera);
  void clear();  // Очистить всех врагов

  // Returns true if player was hit by an enemy
  EnemyCollision checkPlayerCollision(const Player& player);
  virtual void onCollide(Player& player) { player.jump(); };
  // Marks enemy as dead if hit by a projectile AABB
  bool checkProjectileCollision(float px, float py, float pw, float ph);

  std::vector<std::unique_ptr<IEnemy>>& getEnemies() { return enemies; }

  // allow other systems to provide PlatformManager for spawn placement
  void setPlatformManager(class PlatformManager* pm);

  // Set reference to global random number generator
  void setRandomGenerator(std::mt19937& generator) {
    rng = &generator;
    std::cerr << "🟢 [EnemyManager::setRandomGenerator] RNG set" << std::endl;
  }

 private:
  std::vector<std::unique_ptr<IEnemy>> enemies;

  int screenWidth = 0;
  int screenHeight = 0;

  // spawn control
  bool spawnEnabled = false;
  float spawnStartY = 0.0f;  // world Y at which enemies start spawning
  float lastSpawnY = 0.0f;   // world Y of last (most-up) spawn
  float spawnInterval = EnemyConfig::SPAWN_INTERVAL;
  float spawnAhead =
      0.0f;  // how far ahead of player (in world Y) to place new enemies

  // pointer to platform manager to spawn enemies on platforms
  class PlatformManager* platformManager = nullptr;
  EnemyFactory enemyFactory;

  // pointer to global random number generator
  std::mt19937* rng = nullptr;

  void spawnEnemy(float y);

  // Helper methods for cleaner code
  int countAliveEnemies() const;
  void updateEnemiesFromCamera(float dt, int screenWidth, const Camera& camera);
  void cleanupDeadEnemies(const Camera& camera);
  void updateSpawnLogic(float playerY);
  void spawnMissingEnemies(float playerY);
  void spawnInitialBatch(float playerY);

  // Helper for AABB collision detection
  bool checkAABBCollision(float x1, float y1, float w1, float h1, float x2,
                          float y2, float w2, float h2) const;
};
