#pragma once

#include <iostream>

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"
#include "core/Constants.h"

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

  PlatformType type;
  bool active = true;

  virtual void update(float dt, int screenHeight) = 0;
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
    std::cerr << "platform destroyed " << static_cast<int>(type) << std::endl;
    if (sprite) destroySprite(sprite);
  }

 protected:
  int h = PlatformConfig::HEIGHT;
  Sprite* sprite = nullptr;
};
