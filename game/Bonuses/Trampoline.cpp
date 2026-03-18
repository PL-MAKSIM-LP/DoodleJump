#include "Trampoline.h"

#include <iostream>

#include "core/Constants.h"

Trampoline::Trampoline() {
  w = BonusConfig::TRAMPOLINE_W;
  h = BonusConfig::TRAMPOLINE_H;
}

void Trampoline::update(float, int) {}

void Trampoline::applyEffect(Player& player) {
  std::cerr << "[Trampoline::applyEffect]" << std::endl;
  player.jump(BonusConfig::TRAMPOLINE_BOOST_MULT);
}