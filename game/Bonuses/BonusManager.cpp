#include "BonusManager.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "../../framework/Framework.h"
#include "../Platforms/PlatformManager.h"
#include "core/Constants.h"

BonusManager::BonusManager() {}
void BonusManager::update(int screenWidth, int screenHeight,
                          const Camera& camera, const Player& player) {
  std::cerr << "[BonusManager::update] start" << spawnedTestCoin << std::endl;
  if (!spawnedTestCoin) {
    spawnCoin(100, 100);
    spawnedTestCoin = true;
  }

  for (auto& coin : coins) {
    if (coin->isAlive()) {
      coin->checkCollision(const_cast<Player&>(player));
    }
  }

  coins.erase(
      std::remove_if(coins.begin(), coins.end(),
                     [&camera, screenHeight](const std::unique_ptr<IBonus>& b) {
                       // std::cerr
                       // << "[BonusManager::update] remove_if lambda called,
                       // alive="
                       // << b->isAlive() << std::endl;
                       if (!b->isAlive()) {
                         // std::cerr << "[BonusManager::update] Removing dead
                         // coin"
                         //           << std::endl;
                         return true;
                       }
                       float screenY = camera.toScreenY(b->top());
                       if (screenY > screenHeight) {
                         // std::cerr
                         //     << "[BonusManager::update] Removing coin
                         //     off-screen (screenY="
                         //     << screenY << ")" << std::endl;
                         return true;
                       }
                       return false;
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
}

void BonusManager::spawnCoin(int x, int y) {
  std::cerr << "[BonusManager::spawnCoin] Creating coin at x=" << x
            << " y=" << y << std::endl;
  auto coin = bonusFactory.create(x, y, BonusType::COIN);
  std::cerr << "[BonusManager::spawnCoin] coin pointer=" << coin.get()
            << std::endl;
  if (coin) {
    std::cerr << "[BonusManager::spawnCoin] SUCCESS at x=" << x << " y=" << y
              << std::endl;
    coins.push_back(std::move(coin));
  } else {
    std::cerr << "[BonusManager::spawnCoin] FAILED - coin is nullptr"
              << std::endl;
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
}