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

  while ((int)platforms.size() < maxActive) {
    // compute adaptive gap: fewer platforms at higher altitudes
    float gap = computeSpawnGap(lastSpawnY);
    // add a small random multiplier to make gaps less uniform
    float gapRand = std::uniform_real_distribution<float>(0.75f, 1.35f)(gen);
    gap *= gapRand;
    float spawnY = lastSpawnY - gap;

    // pick base X and add jitter so platforms don't align in neat columns
    int baseX = xDist(gen);
    int jitter = std::uniform_int_distribution<int>(
        -static_cast<int>(PlatformConfig::WIDTH),
        static_cast<int>(PlatformConfig::WIDTH))(gen);
    int x = baseX + jitter;
    // clamp to screen bounds
    x = std::max(0, std::min(x, screenWidth - PlatformConfig::WIDTH));

    PlatformType type = selectPlatformType(spawnY);
    spawn(static_cast<float>(x), spawnY, type);
    lastSpawnY = spawnY;

    // Small chance to spawn nearby companion platforms (clusters)
    float clusterP = std::uniform_real_distribution<float>(0.0f, 1.0f)(gen);
    if (clusterP < 0.25f) {
      int offsetX = std::uniform_int_distribution<int>(-120, 120)(gen);
      int cx = std::max(
          0, std::min(x + offsetX, screenWidth - PlatformConfig::WIDTH));
      float cy =
          spawnY - std::uniform_real_distribution<float>(10.0f, 60.0f)(gen);
      PlatformType ctype = selectPlatformType(cy);
      spawn(static_cast<float>(cx), cy, ctype);
    }
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
  // Attempt to find a non-overlapping X position for the new platform.
  int w = PlatformConfig::WIDTH;
  int h = PlatformConfig::HEIGHT;
  float spawnX = x;
  bool placed = false;

  for (int attempt = 0; attempt < SPAWN_RETRY_COUNT; ++attempt) {
    // Clamp candidate X
    int candX =
        std::max(0, std::min(static_cast<int>(spawnX), screenWidth - w));
    if (!overlapsExisting(static_cast<float>(candX), y, w, h)) {
      spawnX = static_cast<float>(candX);
      placed = true;
      break;
    }
    // jitter and try again
    int jitter = std::uniform_int_distribution<int>(-w, w)(gen);
    spawnX = x + jitter;
  }

  if (!placed) {
    // As a fallback try the original X clamped; if it still overlaps, skip
    // spawn
    int candX = std::max(0, std::min(static_cast<int>(x), screenWidth - w));
    if (overlapsExisting(static_cast<float>(candX), y, w, h)) {
      return;  // give up spawning to avoid overlap
    }
    spawnX = static_cast<float>(candX);
  }

  auto platform =
      factory.create(static_cast<int>(spawnX), static_cast<int>(y), type);
  if (platform) platforms.push_back(std::move(platform));
}

bool PlatformManager::overlapsExisting(float x, float y, int w, int h) const {
  for (const auto& p : platforms) {
    if (!p->active) continue;
    // rectangle overlap check
    float px = p->x;
    float py = p->y;
    int pw = p->w;
    int ph = PlatformConfig::HEIGHT;  // use platform height
    bool intersect =
        !(x + w <= px || x >= px + pw || y + h <= py || y >= py + ph);
    if (intersect) return true;
  }
  return false;
}

void PlatformManager::clear() { platforms.clear(); }
