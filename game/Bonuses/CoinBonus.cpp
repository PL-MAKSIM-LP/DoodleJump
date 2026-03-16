#include "CoinBonus.h"

#include "../GameData/GameData.h"
#include "core/Constants.h"

CoinBonus::CoinBonus() {
  w = BonusConfig::COIN_W;
  h = BonusConfig::COIN_H;
}

void CoinBonus::update(float, int) {}

void CoinBonus::applyEffect(Player&) {
  GameData::addCoins(CoinsConfig::COIN_VALUE);
}
