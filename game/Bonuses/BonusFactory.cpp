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
    std::cerr << "BonusFactory::create Loading coin sprite" << std::endl;
    coinSprite = createSprite(BonusConfig::COIN_SPRITE_PATH);
    if (!coinSprite) {
      coinSprite = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
    }
    setSpriteSize(coinSprite, BonusConfig::COIN_W, BonusConfig::COIN_H);
  }

  if (!trampolineSprite) {
    std::cerr << "BonusFactory::create Loading trampoline sprite" << std::endl;
    trampolineSprite = createSprite(BonusConfig::TRAMPOLINE_SPRITE_PATH);
    if (trampolineSprite) {
      setSpriteSize(trampolineSprite, BonusConfig::TRAMPOLINE_W,
                    BonusConfig::TRAMPOLINE_H);
      std::cerr << "Trampoline sprite loaded successfully" << std::endl;
    } else {
      trampolineSprite = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
      std::cerr << "Failed to load trampoline sprite, using fallback"
                << std::endl;
    }
  }

  if (type == BonusType::TRAMPOLINE && trampolineSprite) {
    std::cerr << "BonusFactory::create Creating TRAMPOLINE at x=" << x
              << " y=" << y << std::endl;
    auto bonus = std::make_unique<Trampoline>();
    bonus->init(x, y, trampolineSprite, type);
    return bonus;
  } else if (type == BonusType::COIN && coinSprite) {
    std::cerr << "BonusFactory::create Creating COIN at x=" << x << " y=" << y
              << std::endl;
    auto bonus = std::make_unique<CoinBonus>();
    bonus->init(x, y, coinSprite, type);
    return bonus;
  }

  return nullptr;
}
