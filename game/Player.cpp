#include "Player.h"

static const float GRAVITY    = 1800.0f;  // px/s^2
static const float JUMP_SPEED = -700.0f;  // px/s (negative = up)
static const float MOVE_SPEED = 250.0f;   // px/s

Player::Player() {}

Player::~Player() {
    if (spriteLeft)  destroySprite(spriteLeft);
    if (spriteRight) destroySprite(spriteRight);
}

void Player::init(int screenWidth, int screenHeight) {
    spriteLeft  = createSprite("../data/lik-left.png");
    spriteRight = createSprite("../data/lik-right.png");
    setSpriteSize(spriteLeft,  w, h);
    setSpriteSize(spriteRight, w, h);

    x = screenWidth / 2.0f - w / 2.0f;
    y = screenHeight - 200.0f;

    jump();
}

void Player::update(float dt, int screenWidth) {
    if (!alive) return;

    velX = 0.0f;
    if (movingLeft)  velX = -MOVE_SPEED;
    if (movingRight) velX =  MOVE_SPEED;

    velY += GRAVITY * dt;

    x += velX * dt;
    y += velY * dt;

    // Wrap horizontally
    if (x + w < 0)        x = (float)screenWidth;
    if (x > screenWidth)  x = -(float)w;
}

void Player::render(const Camera& camera) {
    if (!alive) return;

    int screenY = (int)camera.toScreenY(y);
    Sprite* sprite = (velX <= 0.0f) ? spriteLeft : spriteRight;
    drawSprite(sprite, (int)x, screenY);
}

void Player::jump() {
    velY = JUMP_SPEED;
}

void Player::onKeyPressed(FRKey k) {
    if (k == FRKey::LEFT)  movingLeft  = true;
    if (k == FRKey::RIGHT) movingRight = true;
}

void Player::onKeyReleased(FRKey k) {
    if (k == FRKey::LEFT)  movingLeft  = false;
    if (k == FRKey::RIGHT) movingRight = false;
}
