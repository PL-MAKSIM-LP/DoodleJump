#include "BonusManager.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "../../framework/Framework.h"
#include "../Platforms/Platform.h"
#include "../Platforms/PlatformManager.h"
#include "core/Constants.h"

BonusManager::BonusManager() {}
void BonusManager::update(int screenWidth, int screenHeight,
                          const Camera& camera, const Player& player) {
  for (auto& coin : coins) {
    if (coin->isAlive()) {
      coin->checkCollision(const_cast<Player&>(player));
    }
  }

  // Перед удалением монеток - запоминаем на каких платформах они были
  if (platformManager) {
    const auto& platforms = platformManager->getPlatforms();
    for (auto& coin : coins) {
      if (!coin->isAlive()) {
        // Монетка была собрана, найдем на какой платформе она была
        for (const auto& platform : platforms) {
          if (coin->left() >= platform->left() &&
              coin->right() <= platform->right() &&
              coin->bottom() >= platform->top() &&
              coin->bottom() <= platform->bottom()) {
            platformsWithCoins.insert(platform.get());
            break;
          }
        }
      }
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

  spawnCoinsOnPlatforms();
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

void BonusManager::spawnCoinsOnPlatforms() {
  if (!platformManager || !rng) return;

  const auto& platforms = platformManager->getPlatforms();
  constexpr float COIN_SPAWN_CHANCE = 0.30f;

  // Проверяем какие платформы уже имеют живые монетки
  std::set<const IPlatform*> platformsWithLiveCoins;
  for (auto& coin : coins) {
    if (!coin->isAlive()) continue;
    // Проверяем на какой платформе находится монетка
    for (const auto& platform : platforms) {
      if (coin->left() >= platform->left() &&
          coin->right() <= platform->right() &&
          coin->bottom() >= platform->top() &&
          coin->bottom() <= platform->bottom()) {
        platformsWithLiveCoins.insert(platform.get());
        break;
      }
    }
  }

  // Спаунимо новые монетки на платформах которых нет
  for (const auto& platform : platforms) {
    // Монетки только на обычных платформах
    if (platform->type != PlatformType::NORMAL) continue;

    // Пропускаем если на этой платформе уже есть живая монетка
    if (platformsWithLiveCoins.count(platform.get()) > 0) continue;

    // Пропускаем если эта платформа уже отдала монетку (защита от пересоздания)
    if (platformsWithCoins.count(platform.get()) > 0) continue;

    // Генерируем случайное число
    float chance = static_cast<float>((*rng)()) / rng->max();

    // Если меньше 30% - спаунимо монетку
    if (chance < COIN_SPAWN_CHANCE) {
      float coinX = platform->left() +
                    (platform->right() - platform->left()) / 2.0f -
                    static_cast<float>(BonusConfig::COIN_W) / 2.0f;
      float coinY =
          platform->top() - static_cast<float>(BonusConfig::COIN_H) - 5.0f;

      spawnCoin(static_cast<int>(coinX), static_cast<int>(coinY));
      platformsWithCoins.insert(platform.get());
    }
  }
}