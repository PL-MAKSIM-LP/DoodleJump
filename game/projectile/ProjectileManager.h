#pragma once

#include <vector>

#include "../../framework/Framework.h"
#include "../Camera.h"
#include "Projectile.h"

class ProjectileManager {
 public:
  ProjectileManager();
  ~ProjectileManager();

  void init();
  void update(float dt, const Camera& camera);
  void render(const Camera& camera);

  void shoot(float x, float y, float velX = 0.0f, float velY = -600.0f);

  std::vector<Projectile>& getProjectiles() { return projectiles; }

 private:
  std::vector<Projectile> projectiles;
  Sprite* sprite = nullptr;
};
