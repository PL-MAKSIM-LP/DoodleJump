#include "CoinBonus.h"

#include <iostream>

#include "../GameData/GameData.h"
#include "core/Constants.h"

CoinBonus::CoinBonus() {
  w = BonusConfig::COIN_W;
  h = BonusConfig::COIN_H;
}

void CoinBonus::update(float, int) {}

void CoinBonus::applyEffect(Player&) {
  std::cerr << "[CoinBonus::applyEffect] Setting alive=false" << std::endl;
  GameData::addCoins(BonusConfig::COIN_VALUE);
  this->alive = false;
}
