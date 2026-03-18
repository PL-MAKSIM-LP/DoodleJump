#pragma once

#include "../../framework/Framework.h"

class ScoreDisplay {
 public:
  ScoreDisplay() = default;
  ~ScoreDisplay();

  void init();
  void setScore(int s) { score = s; }
  int getScore() const { return score; }
  void reachPlatform() { reached++; }

  void render();

 private:
  static constexpr int DIGIT_WIDTH = 20;
  static constexpr int DIGIT_HEIGHT = 30;

  int reached;
  int score = 0;
  Sprite* digitSprites[10] = {};  // 10 спрайтов для цифр 0-9

  void renderNumber(int number, int x, int y);
  void renderDigit(int digit, int x, int y);
};
;
