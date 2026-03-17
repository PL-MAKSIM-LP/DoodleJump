#pragma once

#include <memory>
#include <random>
#include <vector>

#include "../Camera/Camera.h"
#include "../Platforms/Platform.h"
#include "../Platforms/PlatformFactory.h"
#include "../Player/Player.h"
#include "core/Constants.h"

enum class DifficultyZone { EASY, MEDIUM, HARD, EXTREME };

class PlatformManager {
 public:
  PlatformManager();
  ~PlatformManager();

  void init(int screenWidth, int screenHeight);
  void update(float dt, Player& player);
  void render(const Camera& camera);

  void spawn(float x, float y, PlatformType type);
  void clear();

  PlatformType selectPlatformType(float y);

  const std::vector<std::unique_ptr<IPlatform>>& getPlatforms() const {
    return platforms;
  }

 private:
  void spawnNextPlatform();
  float computeSpawnY();

  // Подметоды для читаемости спавна
  float computeSpawnX();
  std::vector<std::unique_ptr<IPlatform>> platforms;

  // Try this many times to find a non-overlapping spawn X
  static constexpr int SPAWN_RETRY_COUNT = 8;
  bool platformsOverlap(float x, float y, int w, int h) const;

  // Определить зону сложности по Y
  DifficultyZone getDifficultyZone(float y) const;

  std::mt19937 gen;
  PlatformFactory factory;
  int screenWidth = 0;
  int screenHeight = 0;
  float lastPlatformMaxY = 0.0f;  // Y последней спавненой платформы
};