#include "Trampoline.h"

#include "core/Constants.h"

Trampoline::Trampoline() {
  w = BonusConfig::TRAMPOLINE_W;
  h = BonusConfig::TRAMPOLINE_H;
}

void Trampoline::update(float, int) {}

void Trampoline::applyEffect(Player& player) { player.jump(2.0f); }