#include "BonusManager.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "../../framework/Framework.h"
#include "core/Constants.h"

void BonusManager::update(int screenWidth, int screenHeight,
                          const Camera& camera, const Player& player) {
  coins.erase(std::remove_if(coins.begin(), coins.end(),
                             [&player](const std::unique_ptr<IBonus>& b) {
                               return !b->isAlive() ||
                                      b->top() < player.y + 500;
                             }),
              coins.end());

  if (trampoline && !trampoline->isAlive()) {
    std::cerr << "Resetting trampoline" << std::endl;
    trampoline.reset();
    spawnedTrampoline = false;
  }

  float playerScreenY = camera.toScreenY(player.y);
  float worldBottomY =
      player.y + static_cast<float>(screenHeight) - playerScreenY;
  float trampolineY = worldBottomY - BonusConfig::TRAMPOLINE_H;
  int trampolineX =
      static_cast<int>(player.x + static_cast<float>(player.width) / 2.0f -
                       static_cast<float>(BonusConfig::TRAMPOLINE_W) / 2.0f);

  if (spawnedTrampoline) {
    if (!trampoline) {
      spawnTrampoline(trampolineX, trampolineY);
    }
  }

  if (trampoline) {
    trampoline->setPosition(static_cast<float>(trampolineX), trampolineY);

    if (getTickCount() >=
        timeSinceLastTrampoline + BonusConfig::TRAMPOLINE_LIFE_TIME) {
      spawnedTrampoline = false;
      trampoline.reset();
    }
  }

  if (trampoline) {
    if (trampoline->checkCollision(const_cast<Player&>(player))) {
      spawnedTrampoline = false;
    }
  }

  if (coins.size() < coinCapacity) {
    int x = rand() % (screenWidth - BonusConfig::COIN_W);
    float rawY = player.y - 600.0f - static_cast<float>(rand() % screenHeight);
    int coinY = static_cast<int>(rawY);
    spawnCoin(x, coinY);
  }
}

void BonusManager::spawnCoin(int x, int y) {
  auto coin = bonusFactory.create(x, y, BonusType::COIN);
  if (coin) {
    std::cerr << "BonusManager::spawnCoin at x=" << x << " y=" << y
              << std::endl;
    coins.push_back(std::move(coin));
  }
}

void BonusManager::spawnTrampoline(int x, int y) {
  auto bonus = bonusFactory.create(x, y, BonusType::TRAMPOLINE);
  if (bonus) {
    trampoline = std::move(bonus);
    timeSinceLastTrampoline = getTickCount();
    spawnedTrampoline = true;
    std::cerr << "[BonusManager::spawnTrampoline] create" << std::endl;
  } else {
    std::cerr << "[BonusManager::spawnTrampoline] Failed to create"
              << std::endl;
  }

  auto trampoline = bonusFactory.create(x, y, BonusType::TRAMPOLINE);
  if (trampoline) {
    this->trampoline = std::move(trampoline);
    timeSinceLastTrampoline = getTickCount();
  }
}

void BonusManager::render(const Camera& camera) {
  for (auto& coin : coins) {
    coin->render(camera);
  }
  if (trampoline) {
    trampoline->render(camera);
  }
}

void BonusManager::clear() {
  int coinsBefore = coins.size();
  coins.clear();
  if (trampoline) trampoline.reset();
  std::cerr << "[BonusManager::clear] Cleared " << std::endl;

  coins.clear();
  trampoline.reset();
  spawnedTrampoline = false;
  timeSinceLastTrampoline = 0;
}
