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
  xDist = std::uniform_int_distribution<int>(
      0, std::max(0, screenWidth - PlatformConfig::WIDTH));

  float y = screenHeight - PlatformManagerConfig::SPAWN_GAP_Y;
  int x = PlatformManagerConfig::INITIAL_SPAWN_X;
  spawn(static_cast<float>(x), y, PlatformType::NORMAL);

  lastSpawnY = y;
}

void PlatformManager::update(float dt, Player& player) {
  for (auto& p : platforms) {
    if (p->active) p->update(dt, screenWidth);
  }

  float removeY =
      player.y + screenHeight + EnemyPlacementConfig::REMOVE_SCREEN_PADDING;
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
    // compute adaptive gap: fewer platforms at higher altitudes
    float gap = computeSpawnGap(lastSpawnY);
    float spawnY = lastSpawnY - gap;
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

float PlatformManager::computeSpawnGap(float y) const {
  // y decreases as player goes up. Use thresholds to interpolate gap.
  // At low altitude: smaller gap (more platforms). At high altitude: larger
  // gap.
  float lowGap = PlatformManagerConfig::LOW_GAP;  // ~1.25 platforms / 100px
  float midGap = PlatformManagerConfig::MID_GAP;
  float highGap = PlatformManagerConfig::HIGH_GAP;  // ~2 platforms / 300px

  if (y > easyThreshold) {
    return lowGap;
  } else if (y > mediumThreshold) {
    return midGap;
  } else {
    return highGap;
  }
}

PlatformType PlatformManager::selectPlatformType(float y) {
  float r = std::uniform_real_distribution<float>(
      PlatformManagerConfig::RAND_R_MIN,
      PlatformManagerConfig::RAND_R_MAX)(gen);

  if (y > easyThreshold) {
    return PlatformType::NORMAL;
  } else if (y > mediumThreshold) {
    if (r < PlatformManagerConfig::PROB_MEDIUM_NORMAL)
      return PlatformType::NORMAL;
    return PlatformType::MOVING_HORIZONTAL;
  } else if (y > hardThreshold) {
    if (r < PlatformManagerConfig::PROB_VERYHIGH_NORMAL)
      return PlatformType::NORMAL;
    if (r < PlatformManagerConfig::PROB_VERYHIGH_MOVINGH)
      return PlatformType::MOVING_HORIZONTAL;
    return PlatformType::BREAKABLE;
  } else {
    if (r < PlatformManagerConfig::PROB_HIGHEST_NORMAL)
      return PlatformType::NORMAL;
    if (r < PlatformManagerConfig::PROB_HIGHEST_MOVINGH)
      return PlatformType::MOVING_HORIZONTAL;
    if (r < PlatformManagerConfig::PROB_HIGHEST_MOVINGV)
      return PlatformType::MOVING_VERTICAL;
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
