
#include "ProjectileManager.h"

#include <algorithm>
#include <iostream>

#include "core/Constants.h"

ProjectileManager::ProjectileManager() {}

ProjectileManager::~ProjectileManager() {}

void ProjectileManager::init() {
  sprite = createSprite("../data/projectile.png");
  if (!sprite) {
    sprite = createSprite("../data/01-Breakout-Tiles.png");
  }
  if (sprite) setSpriteSize(sprite, ProjectileConfig::W, ProjectileConfig::H);
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
  std::cout << "shoot: " << x << "," << y << " vel=" << velX << "," << velY
            << std::endl;
}

void ProjectileManager::clear() { projectiles.clear(); }
