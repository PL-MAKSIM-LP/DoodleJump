#include "Player.h"

#include <iostream>

#include "core/Constants.h"

Player::Player() {}

Player::~Player() {
  if (spriteLeft) {
    destroySprite(spriteLeft);
  }

  if (spriteRight) {
    destroySprite(spriteRight);
  }

  std::cout << "player destroyed" << std::endl;
}

void Player::init(int screenWidth, int screenHeight) {
  spriteLeft = createSprite(PlayerConfig::SPRITE_LEFT_PATH);
  spriteRight = createSprite(PlayerConfig::SPRITE_RIGHT_PATH);

  if (!spriteLeft) {
    spriteLeft = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
  }
  if (!spriteRight) {
    spriteRight = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
  }

  lastSprite = spriteLeft;

  if (spriteLeft)
    setSpriteSize(spriteLeft, width, height);
  else
    std::cerr << "[Player::init] Failed to load spriteLeft" << std::endl;

  if (spriteRight)
    setSpriteSize(spriteRight, width, height);
  else
    std::cerr << "[Player::init] Failed to load spriteRight" << std::endl;

  x = (screenWidth / 2) - (width / 2);
  y = screenHeight - height;
  this->screenHeight = screenHeight;
}

void Player::update(float dt, int screenWidth, const Camera& camera) {
  velY += GRAVITY * dt;
  y += velY * dt;

  float screenY = camera.toScreenY(y);
  if (screenY > screenHeight) {
    alive = false;
  }
  velX = 0;
  if (movingLeft) {
    velX -= MOVE_SPEED;
    lastSprite = spriteLeft;
  }
  if (movingRight) {
    velX += MOVE_SPEED;
    lastSprite = spriteRight;
  }

  if (x >= screenWidth / 2) {
    velX *= 1.5f;
  }

  x += velX * dt;
  if (x > screenWidth - width / 2) {
    x = 0 - width / 2;
  } else if (x < 0 - width / 2) {
    x = screenWidth - width / 2;
  }
}

void Player::render(const Camera& camera) {
  float screenY = camera.toScreenY(y);

  Sprite* spriteToRender = lastSprite;
  if (velX > 0 && spriteRight) {
    spriteToRender = spriteRight;
  } else if (velX < 0 && spriteLeft) {
    spriteToRender = spriteLeft;
  }

  if (spriteToRender) {
    drawSprite(spriteToRender, x, static_cast<int>(screenY));
  }
}

void Player::jump(float scale) { velY = -JUMP_FORCE * scale; }

void Player::startMoveLeft() { movingLeft = true; };
void Player::stopMoveLeft() { movingLeft = false; };

void Player::startMoveRight() { movingRight = true; };
void Player::stopMoveRight() { movingRight = false; };