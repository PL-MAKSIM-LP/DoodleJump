#pragma once

class MoneyCounter {
 public:
  MoneyCounter() : coins(0) {}

  void addCoins(int amount) { coins += amount; }

  int getCoins() const { return coins; }

  void reset() { coins = 0; }

 private:
  int coins;
};
