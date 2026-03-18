#pragma once
#include <iostream>

#include "core/Constants.h"

class MoneyCounter {
 public:
  MoneyCounter() : coins(BonusConfig::STARTING_COINS) {
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
    coins = BonusConfig::STARTING_COINS;
  }

 private:
  int coins;
};
