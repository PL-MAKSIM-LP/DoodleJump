#pragma once

#include <vector>

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"
#include "Projectile.h"
#include "core/Constants.h"

class ProjectileManager {
 public:
  ProjectileManager();
  ~ProjectileManager();

  void init();
  void update(float dt, const Camera& camera);
  void render(const Camera& camera);
  void clear();  // Очистить все снаряды

  void shoot(float x, float y, float velX = 0.0f,
             float velY = ProjectileConfig::DEFAULT_VELY);

  std::vector<Projectile>& getProjectiles() { return projectiles; }

 private:
  std::vector<Projectile> projectiles;
  Sprite* sprite = nullptr;
};
