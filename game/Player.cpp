#include "Player.h"
#include <iostream>

Player::Player() {}

Player::~Player() {
    if(spriteLeft) {
        destroySprite(spriteLeft);
    }

    if(spriteRight) {
        destroySprite(spriteRight);
    }

    std::cout << "player destroyed" << std::endl;    
}

void Player::init(int screenWidth, int screenHeight) {
    spriteLeft  = createSprite("../data/lik-left.png");
    spriteRight = createSprite("../data/lik-right.png");

    setSpriteSize(spriteLeft, widht, height);
    setSpriteSize(spriteRight, widht, height);

    x = (screenWidth / 2) - (widht / 2);
    y = screenHeight - height; 
}

void Player::update(float dt, int screenWidth) {}

void Player::render(const Camera& camera) {
    drawSprite(spriteRight, x, y);
}

void Player::jump() {}

void Player::onKeyPressed(FRKey k) {}

void Player::onKeyReleased(FRKey k) {}
