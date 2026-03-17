#pragma once

#include <iostream>

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"
#include "../Player/Player.h"

enum class BonusType { COIN, TRAMPOLINE };

class IBonus {
 public:
  virtual ~IBonus() = default;

  virtual void update(float dt, int screenWidth) = 0;
  virtual void applyEffect(Player& player) = 0;

  bool checkCollision(Player& player) {
    if (!alive) return false;

    bool collides =
        !(player.x + player.width <= left() || player.x >= right() ||
          player.y + player.height <= top() || player.y >= bottom());

    if (!collides) return false;

    applyEffect(player);
    return true;
  }

  virtual void render(const Camera& camera) {
    if (alive && sprite) {
      drawSprite(sprite, static_cast<int>(x),
                 static_cast<int>(camera.toScreenY(y)));
    }
  }

  void init(int x, int y, Sprite* spr, BonusType t) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->sprite = spr;
    this->type = t;
  }

  bool isAlive() const { return alive; }
  float left() const { return x; }
  float right() const { return x + w; }
  float top() const { return y; }
  float bottom() const { return y + h; }
  void setPosition(float newX, float newY) {
    x = newX;
    y = newY;
  }

 protected:
  void setAlive(bool value) { alive = value; }

  BonusType type;
  Sprite* sprite = nullptr;
  bool alive = true;
  float x = 0.0f;
  float y = 0.0f;
  int w = 0;
  int h = 0;
};
