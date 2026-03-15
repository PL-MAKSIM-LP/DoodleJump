#pragma once

#include "../../framework/Framework.h"
#include "../Camera.h"

class Projectile {
 public:
  float x = 0.0f;
  float y = 0.0f;
  int w = 10;
  int h = 20;

  bool active = true;

  void update(float dt);
  void render(const Camera& camera, Sprite* sprite);

  float left() const { return x; }
  float right() const { return x + w; }
  float top() const { return y; }
  float bottom() const { return y + h; }

 public:
  float velX = 0.0f;
  float velY = -600.0f;  // default moves upward
};
