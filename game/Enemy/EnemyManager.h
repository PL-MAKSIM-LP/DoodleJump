#pragma once

#include <vector>

#include "../../framework/Framework.h"
#include "../Camera.h"
#include "../Player.h"
#include "Enemy.h"

class EnemyManager {
 public:
  EnemyManager();
  ~EnemyManager();

  void init(int screenWidth, int screenHeight);
  // playerY: current player world Y coordinate (used to decide when to spawn
  // enemies)
  void update(float dt, int screenWidth, const Camera& camera, float playerY);
  void render(const Camera& camera);

  // Returns true if player was hit by an enemy
  bool checkPlayerCollision(const Player& player);

  // Marks enemy as dead if hit by a projectile AABB
  bool checkProjectileCollision(float px, float py, float pw, float ph);

  std::vector<Enemy>& getEnemies() { return enemies; }

 private:
  std::vector<Enemy> enemies;
  Sprite* sprite = nullptr;

  int screenWidth = 0;
  int screenHeight = 0;

  // spawn control
  bool spawnEnabled = false;
  float spawnStartY = 0.0f;  // world Y at which enemies start spawning
  float lastSpawnY = 0.0f;   // world Y of last spawn
  float spawnInterval = 400.0f;

  void spawnEnemy(float y);
};
