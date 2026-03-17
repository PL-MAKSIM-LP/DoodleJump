#include "BonusFactory.h"

#include <iostream>
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
    std::cerr << "🟡 [BonusFactory] Loading coin sprite" << std::endl;
    coinSprite = createSprite("../sprites/coin.png");
    if (coinSprite) {
      setSpriteSize(coinSprite, BonusConfig::COIN_W, BonusConfig::COIN_H);
      std::cerr << "✅ Coin sprite loaded" << std::endl;
    }
  }

  if (!trampolineSprite) {
    std::cerr << "🟡 [BonusFactory] Loading trampoline sprite" << std::endl;
    trampolineSprite = createSprite("../sprites/trampoline.png");
    if (trampolineSprite) {
      setSpriteSize(trampolineSprite, BonusConfig::TRAMPOLINE_W,
                    BonusConfig::TRAMPOLINE_H);
      std::cerr << "✅ Trampoline sprite loaded" << std::endl;
    }
  }

  if (type == BonusType::TRAMPOLINE && trampolineSprite) {
    std::cerr << "🟢 [BonusFactory] Creating TRAMPOLINE at x=" << x << " y="
              << y << std::endl;
    auto bonus = std::make_unique<Trampoline>();
    bonus->init(x, y, trampolineSprite, type);
    return bonus;
  } else if (type == BonusType::COIN && coinSprite) {
    std::cerr << "🟢 [BonusFactory] Creating COIN at x=" << x << " y=" << y
              << std::endl;
    auto bonus = std::make_unique<CoinBonus>();
    bonus->init(x, y, coinSprite, type);
    return bonus;
  }

  return nullptr;
}
