#include "BonusFactory.h"

#include <memory>

#include "CoinBonus.h"
#include "Trampoline.h"
#include "core/Constants.h"

BonusFactory::~BonusFactory() {
  if (coinSprite) {
    destroySprite(coinSprite);
  }
  if (trampolineSprite) {
    destroySprite(trampolineSprite);
  }
}

std::unique_ptr<IBonus> BonusFactory::create(int x, int y, BonusType type) {
  if (!coinSprite) {
    coinSprite = createSprite("../sprites/coin.png");
    if (coinSprite) {
      setSpriteSize(coinSprite, BonusConfig::COIN_W, BonusConfig::COIN_H);
    }
  }

  if (!trampolineSprite) {
    trampolineSprite = createSprite("../sprites/trampoline.png");
    if (trampolineSprite) {
      setSpriteSize(trampolineSprite, BonusConfig::TRAMPOLINE_W,
                    BonusConfig::TRAMPOLINE_H);
    }
  }

  if (type == BonusType::TRAMPOLINE && trampolineSprite) {
    auto bonus = std::make_unique<Trampoline>();
    bonus->init(x, y, trampolineSprite, type);
    return bonus;
  } else if (type == BonusType::COIN && coinSprite) {
    auto bonus = std::make_unique<CoinBonus>();
    bonus->init(x, y, coinSprite, type);
    return bonus;
  }

  return nullptr;
}
