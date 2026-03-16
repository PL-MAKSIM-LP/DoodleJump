#pragma once

#include "../framework/Framework.h"

class ScoreDisplay {
 public:
  ScoreDisplay() = default;
  ~ScoreDisplay();

  void init();
  void setScore(int s) { score = s; }
  int getScore() const { return score; }

  // Отображение счета монет в левом верхнем углу
  void render();

 private:
  int score = 0;
  Sprite* digitSprites[10] = {};  // Спрайты для цифр 0-9

  void renderDigit(int digit, int x, int y);
  void renderNumber(int number, int x, int y);
};
