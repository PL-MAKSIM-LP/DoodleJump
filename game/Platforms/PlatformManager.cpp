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

  float y = screenHeight - PlatformManagerConfig::START_SPAWN_HEIGHT;
  lastPlatformMaxY = y;

  int x = PlatformManagerConfig::INITIAL_SPAWN_X;
  spawn(static_cast<float>(x), y, PlatformType::NORMAL);
}

void PlatformManager::render(const Camera& camera) {
  for (const auto& p : platforms) {
    if (p->active) p->render(camera);
  }
}

void PlatformManager::update(float dt, Player& player) {
  for (auto& p : platforms) {
    if (p->active) p->update(dt, screenHeight);
  }

  float removeY = player.y + screenHeight + EnemyConfig::REMOVE_SCREEN_PADDING;
  platforms.erase(std::remove_if(platforms.begin(), platforms.end(),
                                 [&](const std::unique_ptr<IPlatform>& p) {
                                   if (!p->active) return true;
                                   if (p->y > removeY) {
                                     return true;
                                   }
                                   return false;
                                 }),
                  platforms.end());

  for (const auto& p : platforms) {
    if (p->active && p->isColliding(player)) {
      p->onCollide(player);
    }
  }

  spawnNextPlatform();
}

void PlatformManager::spawnNextPlatform() {
  while (platforms.size() < PlatformManagerConfig::MAX_ACTIVE_PLATFORMS) {
    float spawnY = computeSpawnY();
    PlatformType type = selectPlatformType(spawnY);
    
    float spawnX = 0;
    int retries = SPAWN_RETRY_COUNT;
    do {
      spawnX = computeSpawnX();
    } while (platformsOverlap(spawnX, spawnY, PlatformConfig::WIDTH,
                              PlatformConfig::HEIGHT) && --retries > 0);
    
    // ⚠️ CRITICAL: If retries exhausted, we're spawning on overlapping platform
    if (retries == 0) {
      std::cerr << "⚠️  WARNING: Platform spawned with no free space! Retries="
                << SPAWN_RETRY_COUNT << ", platforms.size=" << platforms.size()
                << std::endl;
    }
    
    spawn(spawnX, spawnY, type);
  }
}

PlatformType PlatformManager::selectPlatformType(float y) {
  float r = std::uniform_real_distribution<float>(
      PlatformManagerConfig::RAND_R_MIN,
      PlatformManagerConfig::RAND_R_MAX)(gen);

  DifficultyZone zone = getDifficultyZone(y);

  switch (zone) {
    case DifficultyZone::EASY:
      return PlatformType::NORMAL;

    case DifficultyZone::MEDIUM:
      return (r < PlatformManagerConfig::PROB_MEDIUM_NORMAL)
                 ? PlatformType::NORMAL
                 : PlatformType::MOVING_HORIZONTAL;

    case DifficultyZone::HARD: {
      if (r < PlatformManagerConfig::PROB_VERYHIGH_NORMAL)
        return PlatformType::NORMAL;
      if (r < PlatformManagerConfig::PROB_VERYHIGH_MOVINGH)
        return PlatformType::MOVING_HORIZONTAL;
      return PlatformType::BREAKABLE;
    }

    case DifficultyZone::EXTREME: {
      if (r < PlatformManagerConfig::PROB_HIGHEST_NORMAL)
        return PlatformType::NORMAL;
      if (r < PlatformManagerConfig::PROB_HIGHEST_MOVINGH)
        return PlatformType::MOVING_HORIZONTAL;
      if (r < PlatformManagerConfig::PROB_HIGHEST_MOVINGV)
        return PlatformType::MOVING_VERTICAL;
      return PlatformType::BREAKABLE;
    }

    default:
      return PlatformType::NORMAL;
  }
}

float PlatformManager::computeSpawnX() {
  float maxX =
      std::max(0.0f, static_cast<float>(screenWidth - PlatformConfig::WIDTH));
  return std::uniform_real_distribution<float>(0.0f, maxX)(gen);
}

float PlatformManager::computeSpawnY() {
  float easyThreshold = static_cast<float>(screenHeight) - PlatformManagerConfig::EASY_OFFSET;
  float mediumThreshold = static_cast<float>(screenHeight) - PlatformManagerConfig::MEDIUM_OFFSET;
  
  float gap = (lastPlatformMaxY > easyThreshold)   ? PlatformManagerConfig::LOW_GAP
              : (lastPlatformMaxY > mediumThreshold) ? PlatformManagerConfig::MID_GAP
                                                       : PlatformManagerConfig::HIGH_GAP;
  
  gap = std::max(gap, PlayerConfig::JumpHeight + 10.0f);

  float next = 0.0f;
  while (next < PlayerConfig::JumpHeight + 50.0f) {
    next = std::uniform_real_distribution<float>(0.0f, gap)(gen);
  }

  lastPlatformMaxY -= next;
  
  // ⚠️ CRITICAL: Y went negative - platforms disappearing off screen
  if (lastPlatformMaxY < 0.0f) {
    std::cerr << "⚠️  WARNING: Negative spawn Y! lastPlatformMaxY=" << lastPlatformMaxY
              << " (gap=" << gap << ", next=" << next << ")" << std::endl;
  }
  
  return lastPlatformMaxY;
}

void PlatformManager::spawn(float x, float y, PlatformType type) {
  auto platform =
      factory.create(static_cast<int>(x), static_cast<int>(y), type);
  if (platform) {
    platforms.push_back(std::move(platform));
  } else {
    // 🔴 CRITICAL: Factory returned nullptr - could cause infinite loop!
    std::cerr << "❌ CRITICAL: PlatformFactory::create() returned nullptr! Type="
              << static_cast<int>(type) << ", platforms.size=" << platforms.size()
              << std::endl;
  }
}

bool PlatformManager::platformsOverlap(float x, float y, int w, int h) const {
  for (const auto& p : platforms) {
    if (!p->active) continue;
    float px = p->x;
    float py = p->y;
    int pw = p->w;
    int ph = PlatformConfig::HEIGHT;
    bool intersect =
        !(x + w <= px || x >= px + pw || y + h <= py || y >= py + ph);
    if (intersect) return true;
  }
  return false;
}

void PlatformManager::clear() { platforms.clear(); }

DifficultyZone PlatformManager::getDifficultyZone(float y) const {
  float easyThreshold = static_cast<float>(screenHeight) - PlatformManagerConfig::EASY_OFFSET;
  float mediumThreshold = static_cast<float>(screenHeight) - PlatformManagerConfig::MEDIUM_OFFSET;
  float hardThreshold = static_cast<float>(screenHeight) - PlatformManagerConfig::HARD_OFFSET;
  
  return (y > easyThreshold)     ? DifficultyZone::EASY
         : (y > mediumThreshold) ? DifficultyZone::MEDIUM
         : (y > hardThreshold)   ? DifficultyZone::HARD
                                 : DifficultyZone::EXTREME;
}
