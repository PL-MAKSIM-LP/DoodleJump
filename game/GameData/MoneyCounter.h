#pragma once
#include "core/Constants.h"

class MoneyCounter {
 public:
  MoneyCounter() : coins(CoinsConfig::START_VALUE) {}

  void addCoins(int amount) { coins += amount; }

  int getCoins() const { return coins; }

  void reset() { coins = CoinsConfig::START_VALUE; }

 private:
  int coins;
};
