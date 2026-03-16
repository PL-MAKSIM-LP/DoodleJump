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
    bool coll = !(player.x + player.width <= e->getX() ||
                  player.x >= e->getX() + e->getW() ||
                  player.y + player.height <= e->getY() ||
                  player.y >= e->getY() + e->getH());
    if (coll) {
      collisionResult = EnemyCollision::HIT;
      if (e->getType() == EnemyType::GROUND && player.velY < 0) {
        collisionResult = EnemyCollision::STOMP;
        e->setAlive(false);
      }
      return collisionResult;
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
        int ey = static_cast<int>(up->y - EnemyConfig::GROUND_H);
        auto e = enemyFactory.create(ex, ey, EnemyType::GROUND);
        enemies.push_back(std::move(e));
        return;
      }
    }
  }

  int x = xDist(gen);
  auto e = enemyFactory.create(x, static_cast<int>(y), EnemyType::FLYING);
  enemies.push_back(std::move(e));
}

void EnemyManager::clear() {
  enemies.clear();
  spawnEnabled = false;
  lastSpawnY = static_cast<float>(screenHeight);
}
