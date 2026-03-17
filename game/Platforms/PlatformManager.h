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

  void setRandomGenerator(std::mt19937& generator) {
    rng = &generator;
    std::cerr << "[PlatformManager::setRandomGenerator] generator set"
              << std::endl;
  }

 private:
  void spawnNextPlatform(float playerY);
  float computeSpawnY();
  float computeSpawnX();
  bool platformsOverlap(float x, float y, int w, int h) const;
  DifficultyZone getDifficultyZone(float y) const;

  static constexpr int SPAWN_RETRY_COUNT = 8;
  std::vector<std::unique_ptr<IPlatform>> platforms;
  std::mt19937* rng = nullptr;
  PlatformFactory factory;
  int screenWidth = 0;
  int screenHeight = 0;
  float lastPlatformMaxY = 0.0f;
};