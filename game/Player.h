#pragma once

#include "../framework/Framework.h"
#include "Camera.h"

class Player {
 public:
  float x = 0.0f;
  float y = 0.0f;
  float velX = 0.0f;
  float velY = 0.0f;
  int width = 50;
  int height = 60;
  const int MOVE_SPEED = 10;

  bool alive = true;

  Player();
  ~Player();

  void init(int screenWidth, int screenHeight);
  void update(float dt, int screenWidth);
  void render(const Camera& camera);

  void startMoveLeft();
  void startMoveRight();
  void stopMoveLeft();
  void stopMoveRight();
  void jump();

 private:
  Sprite* spriteLeft = nullptr;
  Sprite* spriteRight = nullptr;

  bool movingLeft = false;
  bool movingRight = false;
};
