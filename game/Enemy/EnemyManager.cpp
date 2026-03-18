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
  if (!pm) {
    std::cerr
        << "[EnemyManager::setPlatformManager] Platform manager set to null"
        << std::endl;
  }
}

void EnemyManager::init(int w, int h) {
  screenWidth = w;
  screenHeight = h;

  spawnStartY =
      static_cast<float>(screenHeight) - EnemyConfig::SPAWN_START_OFFSET;

  lastSpawnY = static_cast<float>(screenHeight);

  // Определяем позицию спауна в зависимости от сложности
  float enemySpawnY = spawnStartY;
  if (GameConfig::GAME_DIFFICULTY >= GameDifficulty::MEDIUM) {
    // На MEDIUM и выше - спаунимо за пределами экрана (выше верхней границы)
    enemySpawnY = -EnemyConfig::SPAWN_OFFSCREEN_PADDING;
  }

  spawnEnemy(enemySpawnY);
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
  return count;
}

void EnemyManager::updateEnemiesFromCamera(float dt, int screenWidth,
                                           const Camera &camera) {
  for (auto &e : enemies) {
    if (e->isAlive()) e->update(dt, screenWidth);
  }
}

void EnemyManager::cleanupDeadEnemies(const Camera &camera) {
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

void EnemyManager::updateSpawnLogic(float playerY) {}

void EnemyManager::spawnInitialBatch(float playerY) {}

void EnemyManager::spawnMissingEnemies(float playerY) {}

EnemyCollision EnemyManager::checkPlayerCollision(const Player &player) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;

    Object playerObj{static_cast<float>(player.x), static_cast<float>(player.y),
                     static_cast<float>(player.width),
                     static_cast<float>(player.height)};

    Object enemyObj{
        static_cast<float>(e->getX()), static_cast<float>(e->getY()),
        static_cast<float>(e->getW()), static_cast<float>(e->getH())};

    if (checkCollision(playerObj, enemyObj)) {
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

bool EnemyManager::checkProjectileCollision(const Object &projectile) {
  for (auto &e : enemies) {
    if (!e->isAlive()) continue;

    Object enemy{static_cast<float>(e->getX()), static_cast<float>(e->getY()),
                 static_cast<float>(e->getW()), static_cast<float>(e->getH())};

    if (checkCollision(projectile, enemy)) {
      e->setAlive(false);
      return true;
    }
  }
  return false;
}

bool EnemyManager::checkCollision(const Object &obj1,
                                  const Object &obj2) const {
  return !(obj1.x + obj1.w <= obj2.x || obj1.x >= obj2.x + obj2.w ||
           obj1.y + obj1.h <= obj2.y || obj1.y >= obj2.y + obj2.h);
}

void EnemyManager::spawnEnemy(float y) {
  if (!rng) {
    std::cerr << "[EnemyManager::spawnEnemy] Random generator not set!"
              << std::endl;
    return;
  }

  // Спаунимо в случайной X позиции
  std::uniform_int_distribution<int> xDist(
      0, std::max(0, screenWidth - EnemyConfig::X_MARGIN));
  int spawnX = xDist(*rng);

  // Всегда спаунимо летающего врага
  auto enemy =
      enemyFactory.create(spawnX, static_cast<int>(y), EnemyType::FLYING);
  if (enemy) {
    enemies.push_back(std::move(enemy));
    const char *spawnLocation = (y < 0) ? "OFFSCREEN (above)" : "onscreen";
    std::cerr << "[EnemyManager::spawnEnemy] Spawned FLYING enemy at ("
              << spawnX << ", " << y << ") - " << spawnLocation << std::endl;
  }
}

void EnemyManager::clear() {
  enemies.clear();
  lastSpawnY = static_cast<float>(screenHeight);
}
