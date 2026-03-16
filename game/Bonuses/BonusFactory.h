#pragma once

#include <memory>

#include "IBonus.h"

class BonusFactory {
 public:
  BonusFactory() = default;
  ~BonusFactory();
  std::unique_ptr<IBonus> create(int x, int y,
                                 BonusType type = BonusType::COIN);

 private:
  Sprite* coinSprite = nullptr;
  Sprite* trampolineSprite = nullptr;
};
