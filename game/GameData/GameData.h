#pragma once

#include "MoneyCounter.h"

class GameData {
 public:
  GameData() = delete;

  static void addCoins(int amount) { moneyCounter.addCoins(amount); }
  static int getCoins() { return moneyCounter.getCoins(); }
  static void resetCoins() { moneyCounter.reset(); }

 private:
  inline static MoneyCounter moneyCounter{};
};
