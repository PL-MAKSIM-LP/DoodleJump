#pragma once

#include <memory>
#include <random>
#include <vector>

#include "../Camera.h"
#include "../Player.h"
#include "Platform.h"
#include "PlatformFactory.h"

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

 private:
  int screenWidth = 0;
  int screenHeight = 0;

  // spawn density: ~3 platforms per 100px -> gap ~33px
  float spawnGapY = 100.0f;  // base gap (used only as fallback)
  float lastSpawnY = 0.0f;
  // limit active platforms to a reasonable number
  int maxActive = 80;

  // compute adaptive gap based on Y (higher -> larger gap)
  float computeSpawnGap(float y) const;

  std::mt19937 gen;
  std::uniform_int_distribution<int> xDist;

  PlatformFactory factory;

  std::vector<std::unique_ptr<IPlatform>> platforms;
  float easyThreshold = static_cast<float>(screenHeight) - 500.0f;
  float mediumThreshold = static_cast<float>(screenHeight) - 2000.0f;
  float hardThreshold = static_cast<float>(screenHeight) - 4000.0f;
};