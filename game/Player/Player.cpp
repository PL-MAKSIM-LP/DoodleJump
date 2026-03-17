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
  std::cerr << "[Player::init] screenSize=" << screenWidth << "x"
            << screenHeight << std::endl;
  spriteLeft = createSprite("../sprites/lik-left.png");
  spriteRight = createSprite("../sprites/lik-right.png");
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
  std::cerr << "[Player] Initialized at x=" << x << " y=" << y << std::endl;
}

void Player::update(float dt, int screenWidth) {
  velY += GRAVITY * dt;
  y += velY * dt;

  if (y > screenHeight - height + 5) {
    alive = false;
    std::cerr << "[Player::update] DIED " << std::endl;
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
    std::cerr << "[Player::update] swap to left" << std::endl;
    x = 0 - width / 2;
  } else if (x < 0 - width / 2) {
    std::cerr << "[Player::update] swap to right" << std::endl;
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

void Player::jump(float scale) {
  velY = -JUMP_FORCE * scale;
  std::cerr << "[Player::jump] scale=" << scale << std::endl;
}

void Player::startMoveLeft() {
  movingLeft = true;
  std::cerr << "[Player::startMoveLeft]" << std::endl;
};
void Player::stopMoveLeft() {
  movingLeft = false;
  std::cerr << "[Player::stopMoveLeft]" << std::endl;
};

void Player::startMoveRight() {
  movingRight = true;
  std::cerr << "➡️[Player::startMoveRight]" << std::endl;
};
void Player::stopMoveRight() {
  movingRight = false;
  std::cerr << "[Player::stopMoveRight]" << std::endl;
};