#include "EnemyManager.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

#include "EnemyFactory.h"

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

void EnemyManager::init(int w, int h) {
  screenWidth = w;
  screenHeight = h;

  sprite = createSprite("../data/01-Breakout-Tiles.png");
  if (sprite) setSpriteSize(sprite, 50, 40);

  // do not spawn enemies immediately — enable spawning when player reaches
  // height
  spawnEnabled = false;
  spawnStartY = static_cast<float>(screenHeight) -
                2000.0f;  // start spawning after this world Y
  lastSpawnY = static_cast<float>(screenHeight);  // last spawn starts at bottom
}

void EnemyManager::update(float dt, int screenWidth_, const Camera &camera,
                          float playerY) {
  // update enemies (stationary for now)
  for (auto &e : enemies) {
    if (e.alive) e.update(dt, screenWidth_);
  }

  // enable spawning once player reaches the configured start Y
  if (!spawnEnabled && playerY <= spawnStartY) {
    spawnEnabled = true;
    // spawn a few enemies around current player height
    spawnEnemy(playerY - 200.0f);
    lastSpawnY = playerY - 200.0f;
  }

  // if spawning enabled, spawn periodically as player moves up
  if (spawnEnabled) {
    while (lastSpawnY - playerY >= spawnInterval) {
      float nextY = lastSpawnY - spawnInterval;
      spawnEnemy(nextY);
      lastSpawnY = nextY;
    }
  }

  // remove enemies that went off-screen (below visible area)
  enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                               [&](const Enemy &e) {
                                 float screenY = camera.toScreenY(e.y);
                                 if (screenY > screenHeight + 200.0f)
                                   return true;
                                 return !e.alive;
                               }),
                enemies.end());
}

void EnemyManager::render(const Camera &camera) {
  for (auto &e : enemies) {
    if (e.alive) e.render(camera, sprite);
  }
}

bool EnemyManager::checkPlayerCollision(const Player &player) {
  for (auto &e : enemies) {
    if (!e.alive) continue;
    bool coll =
        !(player.x + player.width <= e.left() || player.x >= e.right() ||
          player.y + player.height <= e.top() || player.y >= e.bottom());
    if (coll) return true;
  }
  return false;
}

bool EnemyManager::checkProjectileCollision(float px, float py, float pw,
                                            float ph) {
  for (auto &e : enemies) {
    if (!e.alive) continue;
    bool coll = !(px + pw <= e.left() || px >= e.right() ||
                  py + ph <= e.top() || py >= e.bottom());
    if (coll) {
      e.alive = false;
      return true;
    }
  }
  return false;
}

void EnemyManager::spawnEnemy(float y) {
  static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
  std::uniform_int_distribution<int> xDist(0, std::max(0, screenWidth - 50));
  int x = xDist(gen);

  EnemyFactory factory;
  Enemy e = factory.create(x, static_cast<int>(y));
  enemies.push_back(e);
}
