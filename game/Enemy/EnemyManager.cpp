#include "EnemyManager.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "../Platforms/Platform.h"
#include "../Platforms/PlatformManager.h"
#include "EnemyFactory.h"

// default constructor/destructor are used

void EnemyManager::setPlatformManager(class PlatformManager *pm) {
  platformManager = pm;
  if (pm) {
    std::cerr << "🟢 [EnemyManager::setPlatformManager] Platform manager set"
              << std::endl;
  } else {
    std::cerr
        << "⚠️  [EnemyManager::setPlatformManager] Platform manager set to null"
        << std::endl;
  }
}

void EnemyManager::init(int w, int h) {
  screenWidth = w;
  screenHeight = h;

  spawnEnabled = false;
  spawnStartY =
      static_cast<float>(screenHeight) - EnemyConfig::SPAWN_START_OFFSET;

  spawnAhead =
      static_cast<float>(screenHeight) + EnemyConfig::SPAWN_AHEAD_EXTRA;
  lastSpawnY = static_cast<float>(screenHeight);

  std::cerr << "🟢 [EnemyManager::init] screenSize=" << w << "x" << h
            << " spawnStartY=" << spawnStartY << std::endl;
}

void EnemyManager::update(float dt, int screenWidth_, const Camera &camera,
                          float playerY) {
  updateEnemiesFromCamera(dt, screenWidth_, camera);
  spawnMissingEnemies(playerY);
  updateSpawnLogic(playerY);
  cleanupDeadEnemies(camera);
}

void EnemyManager::render(const Camera &camera) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;
    e->render(camera);
  }
}

int EnemyManager::countAliveEnemies() const {
  int count = 0;
  for (const auto &e : enemies) {
    if (e->isAlive()) ++count;
  }
  if (count != static_cast<int>(enemies.size())) {
    std::cerr << "🟡 [EnemyManager] Alive=" << count
              << " Total=" << enemies.size() << std::endl;
  }
  return count;
}

void EnemyManager::updateEnemiesFromCamera(float dt, int screenWidth,
                                           const Camera &camera) {
  for (auto &e : enemies) {
    if (e->isAlive()) e->update(dt, screenWidth);
  }
}

void EnemyManager::cleanupDeadEnemies(const Camera &camera) {
  int beforeSize = enemies.size();
  enemies.erase(
      std::remove_if(enemies.begin(), enemies.end(),
                     [&](const std::unique_ptr<IEnemy> &e) {
                       float screenY = camera.toScreenY(e->getY());
                       if (screenY >
                           screenHeight + EnemyConfig::REMOVE_SCREEN_PADDING)
                         return true;
                       return !e->isAlive();
                     }),
      enemies.end());
  int afterSize = enemies.size();
  if (beforeSize != afterSize) {
    std::cerr << "🟡 [EnemyManager::cleanupDeadEnemies] Removed "
              << (beforeSize - afterSize) << " enemies (" << beforeSize << "->"
              << afterSize << ")" << std::endl;
  }
}

void EnemyManager::updateSpawnLogic(float playerY) {
  // Enable spawning when player reaches spawn threshold
  if (!spawnEnabled && playerY <= spawnStartY) {
    std::cerr << "🟢 [EnemyManager] Spawn ENABLED at playerY=" << playerY
              << " (threshold=" << spawnStartY << ")" << std::endl;
    spawnEnabled = true;
    spawnInitialBatch(playerY);
  }

  // Continue spawning new enemies as player moves up
  if (spawnEnabled) {
    // Safety limit to prevent infinite loops
    const int maxIterations = 100;
    int iterations = 0;
    while (playerY <= lastSpawnY + spawnInterval &&
           iterations < maxIterations) {
      float nextY = lastSpawnY - spawnInterval;
      spawnEnemy(nextY);
      lastSpawnY = nextY;
      iterations++;
    }
    if (iterations >= maxIterations) {
      std::cerr << "⚠️  EnemyManager: Spawn loop hit iteration limit"
                << std::endl;
    }
  }
}

void EnemyManager::spawnInitialBatch(float playerY) {
  float firstY = playerY - spawnAhead;
  float y = firstY;
  std::cerr << "🟢 [EnemyManager::spawnInitialBatch] Spawning "
            << EnemyConfig::INITIAL_BATCH << " enemies starting at y=" << firstY
            << std::endl;
  for (int i = 0; i < EnemyConfig::INITIAL_BATCH; ++i) {
    spawnEnemy(y);
    y -= spawnInterval;
  }
  lastSpawnY = y + spawnInterval;
}

void EnemyManager::spawnMissingEnemies(float playerY) {
  // Maintain minimum number of enemies
  int aliveCount = countAliveEnemies();
  if (aliveCount < EnemyConfig::DESIRED_ENEMIES) {
    int toSpawn = EnemyConfig::DESIRED_ENEMIES - aliveCount;
    std::cerr << "🟡 [EnemyManager::spawnMissingEnemies] alive=" << aliveCount
              << " desired=" << EnemyConfig::DESIRED_ENEMIES
              << " spawning=" << toSpawn << std::endl;
    for (int i = 0; i < toSpawn; ++i) {
      float spawnY =
          playerY - spawnAhead - (i + 1) * EnemyConfig::SPAWN_INTERVAL;
      spawnEnemy(spawnY);
    }
  }
}

EnemyCollision EnemyManager::checkPlayerCollision(const Player &player) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;

    if (checkAABBCollision(player.x, player.y, player.width, player.height,
                           e->getX(), e->getY(), e->getW(), e->getH())) {
      // Check if player stomped (falling from above)
      float playerCenterY = player.y + static_cast<float>(player.height) / 2.0f;
      float enemyCenterY = e->getY() + static_cast<float>(e->getH()) / 2.0f;
      if (player.velY > 0 && playerCenterY < enemyCenterY) {
        e->setAlive(false);
        std::cerr << "🟢 [EnemyManager] Player STOMPED enemy at x=" << e->getX()
                  << std::endl;
        return EnemyCollision::STOMP;
      }
      std::cerr << "❌ [EnemyManager] Player HIT by enemy at x=" << e->getX()
                << std::endl;
      return EnemyCollision::HIT;
    }
  }
  return EnemyCollision::NOTHING;
}

bool EnemyManager::checkProjectileCollision(float px, float py, float pw,
                                            float ph) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;

    if (checkAABBCollision(px, py, pw, ph, e->getX(), e->getY(), e->getW(),
                           e->getH())) {
      e->setAlive(false);
      std::cerr << "🟢 [EnemyManager] Enemy KILLED by projectile at x="
                << e->getX() << std::endl;
      return true;
    }
  }
  return false;
}

bool EnemyManager::checkAABBCollision(float x1, float y1, float w1, float h1,
                                      float x2, float y2, float w2,
                                      float h2) const {
  return !(x1 + w1 <= x2 || x1 >= x2 + w2 || y1 + h1 <= y2 || y1 >= y2 + h2);
}

void EnemyManager::spawnEnemy(float y) {
  if (!rng) {
    std::cerr << "❌ EnemyManager: Random generator not set!" << std::endl;
    return;
  }

  std::uniform_int_distribution<int> xDist(
      0, std::max(0, screenWidth - EnemyConfig::X_MARGIN));

  auto tooCloseX = [&](int candidateX) {
    for (const auto &enemy : enemies) {
      if (!enemy->isAlive()) continue;
      if (std::abs(static_cast<int>(enemy->getX()) - candidateX) <
          EnemyConfig::MIN_HORIZONTAL_SPACING)
        return true;
    }
    return false;
  };

  // Try to spawn on normal platform
  if (platformManager) {
    const auto &plats = platformManager->getPlatforms();
    for (const auto &plat : plats) {
      if (!plat->active || plat->type != PlatformType::NORMAL) continue;

      if (std::fabs(plat->y - y) <= EnemyConfig::PLATFORM_TOLERANCE) {
        int ex = static_cast<int>(plat->x +
                                  (plat->w - EnemyConfig::GROUND_W) / 2.0f);
        if (tooCloseX(ex)) continue;

        int ey = static_cast<int>(plat->y - EnemyConfig::GROUND_H);
        auto enemy = enemyFactory.create(ex, ey, EnemyType::GROUND);
        if (enemy) {
          enemies.push_back(std::move(enemy));
          std::cerr << "🟢 [EnemyManager::spawnEnemy] GROUNDED at x=" << ex
                    << " y=" << ey << " total=" << enemies.size() << std::endl;
        }
        return;
      }
    }
  }

  // Spawn as flying enemy with spacing constraints
  for (int attempt = 0; attempt < EnemyConfig::SPAWN_X_ATTEMPTS; ++attempt) {
    int x = xDist(*rng);
    if (!tooCloseX(x)) {
      auto enemy =
          enemyFactory.create(x, static_cast<int>(y), EnemyType::FLYING);
      if (enemy) {
        enemies.push_back(std::move(enemy));
        std::cerr << "🟢 [EnemyManager::spawnEnemy] FLYING at x=" << x
                  << " y=" << y << " total=" << enemies.size() << std::endl;
      }
      return;
    }
  }

  std::cerr << "🟡 [EnemyManager::spawnEnemy] Failed to spawn at y=" << y
            << " (spacing constraints)" << std::endl;
}

void EnemyManager::clear() {
  std::cerr << "🟡 [EnemyManager::clear] Clearing " << enemies.size()
            << " enemies" << std::endl;
  enemies.clear();
  spawnEnabled = false;
  lastSpawnY = static_cast<float>(screenHeight);
}
