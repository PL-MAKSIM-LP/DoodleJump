#include "ScoreDisplay.h"

#include <cmath>
#include <iostream>
#include <string>

#include "core/Constants.h"

ScoreDisplay::~ScoreDisplay() {
  for (int i = 0; i < 10; ++i) {
    if (digitSprites[i]) {
      destroySprite(digitSprites[i]);
      digitSprites[i] = nullptr;
    }
  }
}

void ScoreDisplay::init() {
  std::cerr << "[ScoreDisplay::init]" << std::endl;

  for (int i = 0; i < 10; ++i) {
    digitSprites[i] = createSprite(ScoreConfig::DIGIT_SPRITE_PATHS[i]);
    if (digitSprites[i]) {
      setSpriteSize(digitSprites[i], DIGIT_WIDTH, DIGIT_HEIGHT);
      std::cerr << "[ScoreDisplay] Digit " << i << " loaded successfully"
                << std::endl;
    } else {
      digitSprites[i] = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
    }
  }

  BG = createSprite(ScoreConfig::BG_SPRITE_PATH);
  if (BG) {
    setSpriteSize(BG, DIGIT_WIDTH, DIGIT_HEIGHT);
    std::cerr << "[ScoreDisplay] BG loaded successfully" << std::endl;
  } else {
    BG = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
  }

  int bgHeight;
  getScreenSize(bgWidth, bgHeight);
  setSpriteSize(BG, bgWidth, 40);

  reached = 0;
}

void ScoreDisplay::renderDigit(int digit, int x, int y) {
  if (digit < 0 || digit > 9) {
    digit = 0;
  }

  if (digitSprites[digit]) {
    drawSprite(digitSprites[digit], x, y);
  }
}

void ScoreDisplay::renderNumber(int number, int x, int y) {
  if (number < 0) {
    number = 0;
  }

  std::string numStr = std::to_string(number);

  for (size_t i = 0; i < numStr.length(); ++i) {
    int digit = numStr[i] - '0';
    int digitX = x + (i * DIGIT_WIDTH);
    renderDigit(digit, digitX, y);
  }
}

void ScoreDisplay::render() {
  drawSprite(BG, 0, 0);
  renderNumber(score,
               bgWidth / 2 - (DIGIT_WIDTH * std::to_string(score).length()) / 2,
               0);
  renderNumber(reached, 0, 5);
}
