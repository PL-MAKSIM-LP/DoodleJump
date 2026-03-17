#include "ScoreDisplay.h"

#include <cmath>
#include <iostream>
#include <string>

ScoreDisplay::~ScoreDisplay() {
  // Очищаем спрайты цифр
  for (int i = 0; i < 10; i++) {
    if (digitSprites[i]) {
      destroySprite(digitSprites[i]);
    }
  }
}

void ScoreDisplay::init() {
  std::cerr << "🟢 [ScoreDisplay::init]" << std::endl;
  // Загружаем спрайты для цифр (0-9)
  // Используем один файл спрайта где все цифры рядом
  const char* digitFile = "../data/digits.png";

  Sprite* allDigits = createSprite(digitFile);

  if (allDigits) {
    // Предполагаем что спрайты цифр имеют размер примерно 20x30
    setSpriteSize(allDigits, 200, 30);  // 10 цифр по 20px каждая

    // Копируем одиночный спрайт для каждой цифры
    for (int i = 0; i < 10; i++) {
      digitSprites[i] = allDigits;
    }
    std::cerr << "✅ [ScoreDisplay] Digits loaded" << std::endl;
  } else {
    std::cerr << "❌ [ScoreDisplay] Failed to load digits sprite" << std::endl;
  }
}

void ScoreDisplay::renderDigit(int digit, int x, int y) {
  if (digit < 0 || digit > 9) return;

  if (digitSprites[digit]) {
    drawSprite(digitSprites[digit], x, y);
  }
}

void ScoreDisplay::renderNumber(int number, int x, int y) {
  // Преобразуем число в строку цифр и отображаем
  if (number < 0) number = 0;

  std::string numStr = std::to_string(number);
  int digitWidth = 20;  // Ширина одной цифры

  for (size_t i = 0; i < numStr.length(); i++) {
    int digit = numStr[i] - '0';
    renderDigit(digit, x + (i * digitWidth), y);
  }
}

void ScoreDisplay::render() {
  // Отображаем количество монет в левом верхнем углу
  // "Coins: X"
  int screenW = 0, screenH = 0;
  getScreenSize(screenW, screenH);

  // Простое отображение через cout (позже можно заменить на спрайты)
  // Пока это заглушка - нужна функция для отображения текста

  // Отображаем счет в координатах (20, 20)
  renderNumber(score, 20, 20);
}
