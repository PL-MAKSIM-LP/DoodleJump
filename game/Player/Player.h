#pragma once

#include "../../framework/Framework.h"
#include "../Camera/Camera.h"

class Player {
 public:
  float x = 0.0f;
  float y = 0.0f;
  int width = 50;

  float velX = 0.0f;
  float velY = 0.0f;
  int height = 60;

  static constexpr float MOVE_SPEED = 250.0f;
  static constexpr float JUMP_FORCE = 600.0f;
  static constexpr float GRAVITY = 500.0f;
  int screenHeight = 200;

  bool alive = true;

  Player();
  ~Player();

  void init(int screenWidth, int screenHeight);
  void update(float dt, int screenWidth, const Camera& camera);
  void render(const Camera& camera);

  void startMoveLeft();
  void startMoveRight();
  void stopMoveLeft();
  void stopMoveRight();
  void jump(float scale = 1.0f);

 private:
  Sprite* spriteLeft = nullptr;
  Sprite* spriteRight = nullptr;
  Sprite* lastSprite = nullptr;

  bool movingLeft = false;
  bool movingRight = false;
};
