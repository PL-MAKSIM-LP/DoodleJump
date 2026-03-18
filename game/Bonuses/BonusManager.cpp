#include "BonusManager.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "../../framework/Framework.h"
#include "../Platforms/Platform.h"
#include "../Platforms/PlatformManager.h"
#include "core/Constants.h"

void BonusManager::update(int screenWidth, int screenHeight,
                          const Camera& camera, const Player& player) {
  for (auto& coin : coins) {
    if (coin->isAlive()) {
      coin->checkCollision(const_cast<Player&>(player));
    }
  }

  coins.erase(
      std::remove_if(coins.begin(), coins.end(),
                     [&camera, screenHeight](const std::unique_ptr<IBonus>& b) {
                       if (!b->isAlive()) {
                         return true;
                       }
                       float screenY = camera.toScreenY(b->top());
                       if (screenY > screenHeight) {
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

  spawnCoinsOnPlatforms(screenWidth, screenHeight, camera);
}

void BonusManager::spawnCoin(int x, int y) {
  auto coin = bonusFactory.create(x, y, BonusType::COIN);
  if (coin) {
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

void BonusManager::spawnCoinsOnPlatforms(int screenWidth, int screenHeight,
                                         const Camera& camera) {
  if (!rng) return;

  int aliveCoinsCount = 0;
  int screenCoinsCount = 0;
  for (auto& coin : coins) {
    if (!coin->isAlive()) continue;
    aliveCoinsCount++;
    float screenY = camera.toScreenY(coin->top());
    if (screenY >= 0 && screenY < screenHeight) {
      screenCoinsCount++;
    }
  }

  if (aliveCoinsCount >= BonusConfig::MAX_TOTAL_COINS) return;

  if (screenCoinsCount >= BonusConfig::MAX_SCREEN_COINS) return;

  float chance = static_cast<float>((*rng)()) / rng->max();

  float screenCoinX = static_cast<float>((*rng)() % screenWidth);
  float screenCoinY = static_cast<float>((*rng)() % screenHeight);

  // Преобразуем в мировые координаты
  float worldCoinX = screenCoinX;
  float worldCoinY = camera.toWorldY(screenCoinY);

  spawnCoin(static_cast<int>(worldCoinX), static_cast<int>(worldCoinY));
}