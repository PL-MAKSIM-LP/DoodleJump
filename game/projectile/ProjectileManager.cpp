
#include "ProjectileManager.h"

#include <algorithm>
#include <iostream>

#include "core/Constants.h"

ProjectileManager::ProjectileManager() {}

ProjectileManager::~ProjectileManager() {
  // Clean up sprite resource
  if (sprite) {
    destroySprite(sprite);
    sprite = nullptr;
  }
  projectiles.clear();
  std::cerr << "🟢 [ProjectileManager::~ProjectileManager] Cleaned up"
            << std::endl;
}

void ProjectileManager::init() {
  std::cerr << "🟢 [ProjectileManager::init]" << std::endl;
  sprite = createSprite("../data/projectile.png");
  if (!sprite) {
    std::cerr << "⚠️  Fallback to Breakout sprite" << std::endl;
    sprite = createSprite("../data/01-Breakout-Tiles.png");
  }
  if (sprite) {
    setSpriteSize(sprite, ProjectileConfig::W, ProjectileConfig::H);
    std::cerr << "✅ [ProjectileManager] Sprite loaded" << std::endl;
  } else {
    std::cerr << "❌ [ProjectileManager] Failed to load sprite" << std::endl;
  }
}

void ProjectileManager::update(float dt, const Camera &camera) {
  for (auto &p : projectiles) {
    if (p.active) p.update(dt);
  }

  int sw = 0, sh = 0;
  getScreenSize(sw, sh);

  projectiles.erase(
      std::remove_if(projectiles.begin(), projectiles.end(),
                     [&](const Projectile &p) {
                       if (!p.active) return true;
                       float screenY = camera.toScreenY(p.y);
                       if (screenY < -ProjectileConfig::OFFSCREEN_PADDING)
                         return true;
                       if (screenY > sh + ProjectileConfig::OFFSCREEN_PADDING)
                         return true;
                       return false;
                     }),
      projectiles.end());
}

void ProjectileManager::render(const Camera &camera) {
  for (auto &p : projectiles) {
    if (p.active) p.render(camera, sprite);
  }
}

void ProjectileManager::shoot(float x, float y, float velX, float velY) {
  Projectile p;
  p.x = x;
  p.y = y;
  p.velX = velX;
  p.velY = velY;
  p.active = true;
  projectiles.push_back(p);
  std::cerr << "🔫 [ProjectileManager::shoot] x=" << x << " y=" << y
            << " velX=" << velX << " velY=" << velY
            << " total=" << projectiles.size() << std::endl;
}

void ProjectileManager::clear() {
  std::cerr << "🗑️  [ProjectileManager::clear] Cleared " << projectiles.size()
            << " projectiles" << std::endl;
  projectiles.clear();
}
