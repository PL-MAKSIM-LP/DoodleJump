#pragma once

#include "IBonus.h"

class CoinBonus : public IBonus {
 public:
  CoinBonus();

  void update(float dt, int screenWidth) override;
  void applyEffect(Player& player) override;
};
