#include "EnemyManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

#include "../Platforms/Platform.h"
#include "../Platforms/PlatformManager.h"
#include "EnemyFactory.h"

// default constructor/destructor are used

void EnemyManager::setPlatformManager(class PlatformManager *pm) {
  platformManager = pm;
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
}

void EnemyManager::update(float dt, int screenWidth_, const Camera &camera,
                          float playerY) {
  for (auto &e : enemies) {
    if (e->isAlive()) e->update(dt, screenWidth_);
  }

  // Ensure there's a minimum number of active enemies by spawning off-screen
  const int desiredEnemies = 5;
  int aliveCount = 0;
  for (const auto &e : enemies) {
    if (e->isAlive()) ++aliveCount;
  }
  if (aliveCount < desiredEnemies) {
    int toSpawn = desiredEnemies - aliveCount;
    for (int i = 0; i < toSpawn; ++i) {
      // place spawned enemies above the player's view (off-screen)
      float spawnY =
          playerY - spawnAhead - (i + 1) * EnemyConfig::SPAWN_INTERVAL;
      spawnEnemy(spawnY);
    }
  }

  if (!spawnEnabled && playerY <= spawnStartY) {
    spawnEnabled = true;
    float firstY = playerY - spawnAhead;
    const int initialBatch = EnemyConfig::INITIAL_BATCH;
    float y = firstY;
    for (int i = 0; i < initialBatch; ++i) {
      spawnEnemy(y);
      y -= spawnInterval;
    }
    lastSpawnY = y + spawnInterval;
  }

  if (spawnEnabled) {
    while (playerY <= lastSpawnY + spawnInterval) {
      float nextY = lastSpawnY - spawnInterval;
      spawnEnemy(nextY);
      lastSpawnY = nextY;
    }
  }

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
}

void EnemyManager::render(const Camera &camera) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;
    e->render(camera);
  }
}

EnemyCollision EnemyManager::checkPlayerCollision(const Player &player) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;
    auto collisionResult = EnemyCollision::NOTHING;
    // Enemy bounding box: top = e->getY(), bottom = e->getY() + e->getH()
    bool coll = !(player.x + player.width <= e->getX() ||
                  player.x >= e->getX() + e->getW() ||
                  player.y + player.height <= e->getY() ||
                  player.y >= e->getY() + e->getH());
    if (coll) {
      // Prioritize stomp detection: player must be falling and primarily above
      // the enemy. Use vertical centers to decide if player approaches from
      // above.
      float playerCenterY = player.y + static_cast<float>(player.height) / 2.0f;
      float enemyCenterY = e->getY() + static_cast<float>(e->getH()) / 2.0f;
      if (player.velY > 0 && playerCenterY < enemyCenterY) {
        e->setAlive(false);
        return EnemyCollision::STOMP;
      }

      return EnemyCollision::HIT;
    }
  }

  return EnemyCollision::NOTHING;
}

bool EnemyManager::checkProjectileCollision(float px, float py, float pw,
                                            float ph) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;
    bool coll = !(px + pw <= e->getX() || px >= e->getX() + e->getW() ||
                  py + ph <= e->getY() || py >= e->getY() + e->getH());
    if (coll) {
      e->setAlive(false);
      return true;
    }
  }
  return false;
}

void EnemyManager::spawnEnemy(float y) {
  static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
  std::uniform_int_distribution<int> xDist(
      0, std::max(0, screenWidth - EnemyConfig::X_MARGIN));

  auto tooCloseX = [&](int candidateX) {
    const int minSpacing = 500;  // pixels
    for (const auto &exst : enemies) {
      if (!exst->isAlive()) continue;
      if (std::abs(static_cast<int>(exst->getX()) - candidateX) < minSpacing)
        return true;
    }
    return false;
  };

  if (platformManager) {
    const auto &plats = platformManager->getPlatforms();
    const float tolerance = EnemyConfig::PLATFORM_TOLERANCE;
    for (const auto &up : plats) {
      if (!up->active) continue;
      if (up->type != PlatformType::NORMAL) continue;
      float py = up->y;
      if (std::fabs(py - y) <= tolerance) {
        int ex =
            static_cast<int>(up->x + (up->w - EnemyConfig::GROUND_W) / 2.0f);
        // enforce horizontal spacing: skip this platform if too close
        if (tooCloseX(ex)) continue;
        int ey = static_cast<int>(up->y - EnemyConfig::GROUND_H);
        auto e = enemyFactory.create(ex, ey, EnemyType::GROUND);
        enemies.push_back(std::move(e));
        return;
      }
    }
  }

  // Flying enemy spawn: try several attempts to find X with required spacing
  const int maxAttempts = 8;
  int chosenX = -1;
  for (int attempt = 0; attempt < maxAttempts; ++attempt) {
    int x = xDist(gen);
    if (!tooCloseX(x)) {
      chosenX = x;
      break;
    }
  }
  if (chosenX == -1) return;  // give up spawning to maintain spacing

  auto e = enemyFactory.create(chosenX, static_cast<int>(y), EnemyType::FLYING);
  if (e) enemies.push_back(std::move(e));
}

void EnemyManager::clear() {
  enemies.clear();
  spawnEnabled = false;
  lastSpawnY = static_cast<float>(screenHeight);
}
