#pragma once

#include "IBonus.h"

class Trampoline : public IBonus {
 public:
  Trampoline();

  void update(float dt, int screenWidth) override;
  void applyEffect(Player& player) override;
};
