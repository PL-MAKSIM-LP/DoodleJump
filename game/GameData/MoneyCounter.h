#pragma once
#include <iostream>

#include "core/Constants.h"

class MoneyCounter {
 public:
  MoneyCounter() : coins(CoinsConfig::START_VALUE) {
    std::cerr << "[MoneyCounter::constructor] Created with coins=" << coins
              << std::endl;
  }

  void addCoins(int amount) {
    int oldCoins = coins;
    coins += amount;
    std::cerr << "[MoneyCounter::addCoins] +" << amount
              << " New value: " << coins << std::endl;
  }

  int getCoins() const { return coins; }

  void reset() {
    std::cerr << "[MoneyCounter::reset]" << std::endl;
    coins = CoinsConfig::START_VALUE;
  }

 private:
  int coins;
};
