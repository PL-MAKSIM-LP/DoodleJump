#pragma once

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"
#include "core/Constants.h"

class Projectile {
 public:
  float x = 0.0f;
  float y = 0.0f;

  bool active = true;
  Projectile();
  void update(float dt);
  void render(const Camera& camera, Sprite* sprite);

  float left() const { return x; }
  float right() const { return x + w; }
  float top() const { return y; }
  float bottom() const { return y + h; }

 public:
  unsigned int spawnTime = 0;
  float velX = 0.0f;
  float velY = ProjectileConfig::DEFAULT_VELY;

 private:
  int w = ProjectileConfig::W;
  int h = ProjectileConfig::H;
};
