#pragma once

#include <memory>
#include <vector>

#include "../Camera.h"
#include "../Player.h"
#include "EnemyFactory.h"
#include "IEnemy.h"

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

  void spawnEnemy(float y);
};
