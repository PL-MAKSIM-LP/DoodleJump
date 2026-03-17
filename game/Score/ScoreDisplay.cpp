#include "ScoreDisplay.h"

#include <cmath>
#include <iostream>
#include <string>

ScoreDisplay::~ScoreDisplay() {
  // Очищаем спрайт цифр (только один спрайт, остальные NULL)
  if (digitSprites[0]) {
    destroySprite(digitSprites[0]);
    digitSprites[0] = nullptr;
  }
  // Обнулить все остальные для безопасности
  for (int i = 1; i < 10; i++) {
    digitSprites[i] = nullptr;
  }
}

void ScoreDisplay::init() {
  std::cerr << "🟢 [ScoreDisplay::init]" << std::endl;
  // Загружаем один спрайт для всех цифр (0-9)
  const char* digitFile = "../data/digits.png";

  // Загружаем один спрайт, который содержит все цифры
  digitSprites[0] = createSprite(digitFile);

  if (digitSprites[0]) {
    // Предполагаем что спрайты цифр имеют размер примерно 20x30
    setSpriteSize(digitSprites[0], 200, 30);  // 10 цифр по 20px каждая

    // Все остальные индексы указывают на NULL, так как мы используем subregions
    for (int i = 1; i < 10; i++) {
      digitSprites[i] = nullptr;
    }
    std::cerr << "✅ [ScoreDisplay] Digits loaded (single sprite for all)"
              << std::endl;
  } else {
    std::cerr << "❌ [ScoreDisplay] Failed to load digits sprite" << std::endl;
    // Обнулить все спрайты в случае ошибки
    for (int i = 0; i < 10; i++) {
      digitSprites[i] = nullptr;
    }
  }
}

void ScoreDisplay::renderDigit(int digit, int x, int y) {
  if (digit < 0 || digit > 9) {
    std::cerr << "⚠️  [ScoreDisplay::renderDigit] Invalid digit: " << digit
              << std::endl;
    return;
  }

  if (digitSprites[digit]) {
    drawSprite(digitSprites[digit], x, y);
  }
  // No logging for valid digits - render is called every frame
}

void ScoreDisplay::renderNumber(int number, int x, int y) {
  // Преобразуем число в строку цифр и отображаем
  if (number < 0) {
    std::cerr << "⚠️  [ScoreDisplay::renderNumber] Negative number: " << number
              << std::endl;
    number = 0;
  }

  std::string numStr = std::to_string(number);
  int digitWidth = 20;  // Ширина одной цифры

  for (size_t i = 0; i < numStr.length(); i++) {
    int digit = numStr[i] - '0';
    renderDigit(digit, x + (i * digitWidth), y);
  }
  // No logging for valid numbers - render is called every frame
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
