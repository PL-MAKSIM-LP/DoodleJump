#include "CoinBonus.h"

#include "BonusConfig.h"

CoinBonus::CoinBonus() {
  w = BonusConfig::COIN_W;
  h = BonusConfig::COIN_H;
}

void CoinBonus::update(float, int) {}

void CoinBonus::applyEffect(Player&) {}
