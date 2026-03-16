#pragma once

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"
#include "core/Constants.h"

enum class EnemyType { GROUND, FLYING };

class IEnemy {
 public:
  virtual ~IEnemy() = default;

  virtual void update(float dt, int screenWidth) = 0;
  virtual void render(const Camera& camera) {
    if (sprite) {
      drawSprite(sprite, static_cast<int>(x),
                 static_cast<int>(camera.toScreenY(y)));
    }
  };

  virtual void init(int x, int y, Sprite* spr, EnemyType t) {
    this->x = x;
    this->y = y;
    this->sprite = spr;
    this->type = t;
    if (type == EnemyType::FLYING) {
      w = EnemyConfig::FLY_W;
      h = EnemyConfig::FLY_H;
    } else {
      w = EnemyConfig::GROUND_W;
      h = EnemyConfig::GROUND_H;
    }
  }

  virtual bool isAlive() const { return alive; }
  virtual void setAlive(bool a) { alive = a; }

  virtual float getX() const { return x; }
  virtual float getY() const { return y; }
  virtual int getW() const { return w; }
  virtual int getH() const { return h; }
  virtual EnemyType getType() const { return type; }

 protected:
  EnemyType type;
  Sprite* sprite = nullptr;
  bool alive = true;
  float x = 0.0f;
  float y = 0.0f;
  int w = 0;
  int h = 0;
};
