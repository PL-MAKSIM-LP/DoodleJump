#include "Trampoline.h"

#include <iostream>

#include "core/Constants.h"

Trampoline::Trampoline() {
  w = BonusConfig::TRAMPOLINE_W;
  h = BonusConfig::TRAMPOLINE_H;
}

void Trampoline::update(float, int) {
  // No significant state changes to log
}

void Trampoline::applyEffect(Player& player) {
  std::cerr << "🪂 [Trampoline::applyEffect] Boost jump" << std::endl;
  player.jump(BonusConfig::TRAMPOLINE_BOOST_MULT);
}