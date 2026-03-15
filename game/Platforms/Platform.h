#pragma once

#include "../../framework/Framework.h"
#include "../Camera.h"
#include "../Constants.h"
#include "../Player.h"

enum class PlatformType {
  NORMAL,
  MOVING_HORIZONTAL,
  MOVING_VERTICAL,
  BREAKABLE
};

class IPlatform {
 public:
  float x = 0.0f;
  float y = 0.0f;
  int w = PlatformConfig::WIDTH;
  int h = PlatformConfig::HEIGHT;
  Sprite* sprite = nullptr;

  PlatformType type;
  bool active = true;

  virtual void update(float dt, int screenWidth) = 0;
  virtual void render(const Camera& camera);
  virtual void onCollide(Player& player) { player.jump(); };
  virtual void reset(int x, int y);
  void init(int x, int y, Sprite* spr, PlatformType t);

  float left() const { return x; }
  float right() const { return x + w; }
  float top() const { return y; }
  float bottom() const { return y + h; }

  bool isColliding(const Player& player) const;

  virtual ~IPlatform() {
    if (sprite) destroySprite(sprite);
  }
};
