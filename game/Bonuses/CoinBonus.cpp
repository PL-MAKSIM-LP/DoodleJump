#include "CoinBonus.h"

#include <iostream>

#include "../GameData/GameData.h"
#include "core/Constants.h"

CoinBonus::CoinBonus() {
  w = BonusConfig::COIN_W;
  h = BonusConfig::COIN_H;
}

void CoinBonus::update(float, int) {
  // Coins don't have movement - gravity is handled by BonusManager
}

void CoinBonus::applyEffect(Player&) {
  GameData::addCoins(CoinsConfig::COIN_VALUE);
  std::cerr << "[CoinBonus::applyEffect]" << std::endl;
}
