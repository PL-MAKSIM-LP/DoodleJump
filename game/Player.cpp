#include "Player.h"

#include <iostream>

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
  spriteLeft = createSprite("../data/lik-left.png");
  spriteRight = createSprite("../data/lik-right.png");
  lastSprite = spriteLeft;

  if (spriteLeft) setSpriteSize(spriteLeft, width, height);
  if (spriteRight) setSpriteSize(spriteRight, width, height);

  x = (screenWidth / 2) - (width / 2);
  y = screenHeight - height;
  this->screenHeight = screenHeight;
}

void Player::update(float dt, int screenWidth) {
  velY += GRAVITY * dt;
  y += velY * dt;

  if (y > screenHeight - height + 10) {
    alive = false;
    jump();
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

  x += velX * dt;
  if (x > screenWidth - width / 2) {
    x = 0 - width / 2;
  } else if (x < 0 - width / 2) {
    x = screenWidth - width / 2;
  }
}

void Player::render(const Camera& camera) {
  float screenY = camera.toScreenY(y);
  if (velX > 0) {
    drawSprite(spriteRight, x, static_cast<int>(screenY));
  } else if (velX < 0) {
    drawSprite(spriteLeft, x, static_cast<int>(screenY));
  } else {
    drawSprite(lastSprite, x, static_cast<int>(screenY));
  }
}

void Player::jump() { velY = -JUMP_FORCE; }

void Player::startMoveLeft() { movingLeft = true; };
void Player::stopMoveLeft() { movingLeft = false; };

void Player::startMoveRight() { movingRight = true; };
void Player::stopMoveRight() { movingRight = false; };