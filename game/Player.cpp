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

  setSpriteSize(spriteLeft, width, height);
  setSpriteSize(spriteRight, width, height);

  x = (screenWidth / 2) - (width / 2);
  y = screenHeight - height;
}

void Player::update(float dt, int screenWidth) {
  velX = 0;
  if (movingLeft) {
    velX -= MOVE_SPEED;
  }
  if (movingRight) {
    velX += MOVE_SPEED;
  }

  x += velX * dt;
}

void Player::render(const Camera& camera) {
  if (velX > 0) {
    drawSprite(spriteRight, x, y);
  } else {
    drawSprite(spriteLeft, x, y);
  }
}

void Player::jump() {}

void Player::startMoveLeft() { movingLeft = true; };
void Player::stopMoveLeft() { movingLeft = false; };

void Player::startMoveRight() { movingRight = true; };
void Player::stopMoveRight() { movingRight = false; };