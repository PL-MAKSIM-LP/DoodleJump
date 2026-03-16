#include "BonusManager.h"

#include <algorithm>
#include <cstdlib>

#include "BonusConfig.h"

void BonusManager::update(int screenWidth, int screenHeight,
                          const Camera& camera, float playerY) {
  coins.erase(std::remove_if(coins.begin(), coins.end(),
                             [playerY](const std::unique_ptr<IBonus>& b) {
                               return !b->isAlive() || b->top() < playerY + 500;
                             }),
              coins.end());

  if (trampoline && !trampoline->isAlive()) {
    trampoline.reset();
  }

  float playerScreenY = camera.toScreenY(playerY);
  float worldBottomY =
      playerY + static_cast<float>(screenHeight) - playerScreenY;
  float trampolineY = worldBottomY - BonusConfig::TRAMPOLINE_H;
  int trampolineX = (screenWidth - BonusConfig::TRAMPOLINE_W) / 2;

  if (!trampoline) {
    spawnTrampoline(trampolineX, trampolineY);
  } else {
    trampoline->setPosition(static_cast<float>(trampolineX), trampolineY);
  }

  if (coins.size() < coinCapacity) {
    int x = rand() % (screenWidth - BonusConfig::COIN_W);
    float y = playerY - 600.0f - (rand() % screenHeight);
    spawnCoin(x, y);
  }
}

void BonusManager::spawnCoin(int x, float y) {
  auto coin = bonusFactory.create(x, static_cast<int>(y), BonusType::COIN);
  if (coin) {
    coins.push_back(std::move(coin));
  }
}

void BonusManager::spawnTrampoline(int x, float y) {
  auto trampoline =
      bonusFactory.create(x, static_cast<int>(y), BonusType::TRAMPOLINE);
  if (trampoline) {
    this->trampoline = std::move(trampoline);
  }
}
