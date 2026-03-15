#include "PlatformManager.h"

#include <algorithm>
#include <ctime>
#include <iostream>

PlatformManager::PlatformManager()
    : gen(static_cast<unsigned int>(std::time(nullptr))) {}

PlatformManager::~PlatformManager() { clear(); }

void PlatformManager::init(int screenWidth_, int screenHeight_) {
  screenWidth = screenWidth_;
  screenHeight = screenHeight_;
  xDist = std::uniform_int_distribution<int>(0, std::max(0, screenWidth - 70));

  float y = screenHeight - 100.0f;
  int x = 100;
  spawn(static_cast<float>(x), y, PlatformType::NORMAL);

  lastSpawnY = y;
}

void PlatformManager::update(float dt, Player& player) {
  for (auto& p : platforms) {
    if (p->active) p->update(dt, screenWidth);
  }

  float removeY = player.y + screenHeight + 200.0f;
  platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
                                 [&](const std::unique_ptr<IPlatform>& p) {
                                   if (!p->active) return true;
                                   if (p->y > removeY) {
                                     return true;
                                   }
                                   return false;
                                 }),
                  platforms.end());

  while ((int)platforms.size() < maxActive) {
    float spawnY = lastSpawnY - spawnGapY;
    int x = xDist(gen);
    PlatformType type = selectPlatformType(spawnY);
    spawn(static_cast<float>(x), spawnY, type);
    lastSpawnY = spawnY;
  }

  for (const auto& p : platforms) {
    if (p->active && p->isColliding(player)) {
      p->onCollide(player);
    }
  }
}

PlatformType PlatformManager::selectPlatformType(float y) {
  float r = std::uniform_real_distribution<float>(0.0f, 1.0f)(gen);

  if (y > easyThreshold) {
    return PlatformType::NORMAL;
  } else if (y > mediumThreshold) {
    if (r < 0.80f) return PlatformType::NORMAL;
    return PlatformType::MOVING_HORIZONTAL;
  } else if (y > hardThreshold) {
    if (r < 0.60f) return PlatformType::NORMAL;
    if (r < 0.90f) return PlatformType::MOVING_HORIZONTAL;
    return PlatformType::BREAKABLE;
  } else {
    if (r < 0.40f) return PlatformType::NORMAL;
    if (r < 0.75f) return PlatformType::MOVING_HORIZONTAL;
    if (r < 0.90f) return PlatformType::MOVING_VERTICAL;
    return PlatformType::BREAKABLE;
  }
}

void PlatformManager::render(const Camera& camera) {
  for (const auto& p : platforms) {
    if (p->active) p->render(camera);
  }
}

void PlatformManager::spawn(float x, float y, PlatformType type) {
  std::cout << "spawn" << std::endl;
  std::cout << x << " " << y << std::endl;

  auto platform =
      factory.create(static_cast<int>(x), static_cast<int>(y), type);
  platforms.push_back(std::move(platform));
}

void PlatformManager::clear() { platforms.clear(); }
