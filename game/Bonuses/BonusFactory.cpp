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
    coinSprite = createSprite(BonusConfig::COIN_SPRITE_PATH);
    if (!coinSprite) {
      coinSprite = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
    }
    setSpriteSize(coinSprite, BonusConfig::COIN_W, BonusConfig::COIN_H);
  }

  if (!trampolineSprite) {
    trampolineSprite = createSprite(BonusConfig::TRAMPOLINE_SPRITE_PATH);
    if (!trampolineSprite) {
      trampolineSprite = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
      std::cerr << "[BonusFactory] Failed to load trampoline sprite"
                << std::endl;
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
