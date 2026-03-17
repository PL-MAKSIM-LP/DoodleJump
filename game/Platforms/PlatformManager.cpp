#include "PlatformManager.h"

#include <algorithm>
#include <iostream>

PlatformManager::PlatformManager() {}

PlatformManager::~PlatformManager() { clear(); }

// TODO: сделать пул платформ и просто менять их позиции

void PlatformManager::init(int screenWidth_, int screenHeight_) {
  screenWidth = screenWidth_;
  screenHeight = screenHeight_;

  float y = screenHeight - PlatformManagerConfig::START_SPAWN_HEIGHT;
  lastPlatformMaxY = y;

  int x = PlatformManagerConfig::INITIAL_SPAWN_X;
  spawn(static_cast<float>(x), y, PlatformType::NORMAL);
  std::cerr << "[PlatformManager::init] screen=" << screenWidth << "x"
            << screenHeight << std::endl;
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
  int beforeSize = platforms.size();
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
  std::cerr << "[PlatformManager::spawnNextPlatform] start" << std::endl;
  while (platforms.size() < PlatformManagerConfig::MAX_ACTIVE_PLATFORMS) {
    float spawnY = computeSpawnY();
    PlatformType type = selectPlatformType(spawnY);

    float spawnX = 0;
    int retries = SPAWN_RETRY_COUNT;
    do {
      spawnX = computeSpawnX();
      --retries;
    } while (platformsOverlap(spawnX, spawnY, PlatformConfig::WIDTH,
                              PlatformConfig::HEIGHT) &&
             retries > 0);
    spawn(spawnX, spawnY, type);
  }
  std::cerr << "[PlatformManager::spawnNextPlatform] end" << std::endl;
}

float PlatformManager::computeSpawnX() {
  if (!rng) {
    std::cerr << "[PlatformManager::computeSpawnX] generator is not set!"
              << std::endl;
    return static_cast<float>(screenWidth / 2);
  }
  float maxX =
      std::max(0.0f, static_cast<float>(screenWidth - PlatformConfig::WIDTH));
  return std::uniform_real_distribution<float>(0.0f, maxX)(*rng);
}

float PlatformManager::computeSpawnY() {
  float easyThreshold =
      static_cast<float>(screenHeight) - PlatformManagerConfig::EASY_OFFSET;
  float mediumThreshold =
      static_cast<float>(screenHeight) - PlatformManagerConfig::MEDIUM_OFFSET;

  float gap =
      (lastPlatformMaxY > easyThreshold)     ? PlatformManagerConfig::LOW_GAP
      : (lastPlatformMaxY > mediumThreshold) ? PlatformManagerConfig::MID_GAP
                                             : PlatformManagerConfig::HIGH_GAP;

  gap = std::max(gap, PlayerConfig::JumpHeight + 10.0f);

  if (!rng) {
    std::cerr << "[PlatformManager::computeSpawnY] generator is not set!"
              << std::endl;
    return lastPlatformMaxY - gap;
  }

  float next = 0.0f;
  std::cerr
      << "[PlatformManager::computeSpawnY] PlayerConfig::JumpHeight + 50.0f = "
      << PlayerConfig::JumpHeight << std::endl;

  while (true) {
    next = std::uniform_real_distribution<float>(0.0f, gap)(*rng);

    std::cerr << "[PlatformManager::computeSpawnY] next = " << next
              << std::endl;
    if (next >= PlayerConfig::JumpHeight) {
      break;
    }
  }

  lastPlatformMaxY -= next;

  if (lastPlatformMaxY < 0.0f) {
    std::cerr << " WARNING: Negative spawn Y! lastPlatformMaxY="
              << lastPlatformMaxY << std::endl;
  }

  return lastPlatformMaxY;
}

void PlatformManager::spawn(float x, float y, PlatformType type) {
  std::cerr << "[PlatformFactory::spawn]  x=" << x << " y=" << y
            << " type=" << static_cast<int>(type) << std::endl;
  auto platform =
      factory.create(static_cast<int>(x), static_cast<int>(y), type);
  if (platform) {
    platforms.push_back(std::move(platform));
  } else {
    std::cerr << "[PlatformFactory::create] returned nullptr!" << std::endl;
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
  float easyThreshold =
      static_cast<float>(screenHeight) - PlatformManagerConfig::EASY_OFFSET;
  float mediumThreshold =
      static_cast<float>(screenHeight) - PlatformManagerConfig::MEDIUM_OFFSET;
  float hardThreshold =
      static_cast<float>(screenHeight) - PlatformManagerConfig::HARD_OFFSET;

  return (y > easyThreshold)     ? DifficultyZone::EASY
         : (y > mediumThreshold) ? DifficultyZone::MEDIUM
         : (y > hardThreshold)   ? DifficultyZone::HARD
                                 : DifficultyZone::EXTREME;
}

PlatformType PlatformManager::selectPlatformType(float y) {
  if (!rng) {
    std::cerr << "[PlatformManager::selectPlatformType] generator is not set!"
              << std::endl;

    return PlatformType::NORMAL;
  }
  float r = std::uniform_real_distribution<float>(
      PlatformManagerConfig::RAND_R_MIN,
      PlatformManagerConfig::RAND_R_MAX)(*rng);

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