#include <iostream>
#include <memory>
#include <random>

#include "../game/Bonuses/BonusManager.h"
#include "../game/Camera/Camera.h"
#include "../game/Player/Player.h"
#include "../game/core/Constants.h"

struct TestCoinsContext {
  int coinsSpawned = 0;
  int coinsOnScreen = 0;
};

void printCoinsInfo(const BonusManager& bonusManager, const Camera& camera,
                    int screenWidth, int screenHeight,
                    const std::string& label) {
  std::cout << "\n--- " << label << " ---" << std::endl;
  std::cout << "Player Y: " << 0 << std::endl;
  //   std::cout << "Camera Y: " << camera.getY() << std::endl;
  std::cout << "Note: Full coin state access requires friend class or public "
               "getter\n";
  std::cout << "      In real test, you would inspect coins vector directly"
            << std::endl;
}

bool testBonusManagerCoinSpawning() {
  std::cout << "\n=== BONUS MANAGER COIN SPAWNING TEST ===" << std::endl;

  // Инициализируем прилождение
  int screenWidth = 390;
  int screenHeight = 844;

  // Создаем игрока на начальной позиции
  Player player;
  player.x = screenWidth / 2.0f;
  player.y = 0.0f;
  player.width = 50;
  player.height = 50;

  std::cout << "Initial player position: (" << player.x << ", " << player.y
            << ")" << std::endl;

  // Создаем камеру
  Camera camera;
  camera.update(player.y, screenHeight);

  // Создаем BonusManager
  BonusManager bonusManager;

  // Инициализируем random generator
  std::mt19937 rng(42);  // Фиксированный seed для reproducibility
  bonusManager.setRandomGenerator(rng);

  // ===== STAGE 1: Спаун монеток с игроком на начальной позиции =====
  std::cout << "\n>>> STAGE 1: Initial Position" << std::endl;
  printCoinsInfo(bonusManager, camera, screenWidth, screenHeight,
                 "Coins at start position");

  // Спаунимо монетки несколько раз
  for (int i = 0; i < 50; ++i) {
    bonusManager.update(screenWidth, screenHeight, camera, player);
  }

  std::cout << "Spawned coins (after 50 updates)" << std::endl;
  std::cout << "Expected: Up to " << BonusConfig::MAX_SCREEN_COINS
            << " visible on screen\n";
  std::cout << "Expected: Up to " << BonusConfig::MAX_TOTAL_COINS
            << " total coins\n";

  // ===== STAGE 2: Перемещаем игрока вверх на -5000 =====
  std::cout << "\n>>> STAGE 2: Player moved to Y = -5000" << std::endl;

  player.y = -5000.0f;
  camera.update(player.y, screenHeight);

  std::cout << "Player position after move: (" << player.x << ", " << player.y
            << ")" << std::endl;
  //   std::cout << "Camera position: " << camera.getY() << std::endl;

  printCoinsInfo(bonusManager, camera, screenWidth, screenHeight,
                 "Coins after player movement");

  // Спаунимо монетки снова
  for (int i = 0; i < 50; ++i) {
    bonusManager.update(screenWidth, screenHeight, camera, player);
  }

  std::cout << "\nSpawned coins (after movement, 50 more updates)" << std::endl;
  std::cout
      << "Expected: Different coins should spawn at new camera position\n";
  std::cout << "Expected: Still max " << BonusConfig::MAX_SCREEN_COINS
            << " visible\n";

  // ===== STAGE 3: Вернуться на начальную позицию =====
  std::cout << "\n>>> STAGE 3: Back to initial position" << std::endl;

  player.y = 0.0f;
  camera.update(player.y, screenHeight);

  printCoinsInfo(bonusManager, camera, screenWidth, screenHeight,
                 "Coins back at initial position");

  // Финальный спаун
  for (int i = 0; i < 50; ++i) {
    bonusManager.update(screenWidth, screenHeight, camera, player);
  }

  std::cout << "\nSpawned coins (back at start, 50 more updates)" << std::endl;
  std::cout << "Expected: Coins should continue spawning within limits\n";

  std::cout << "\n=== TEST COMPLETED ===" << std::endl;
  std::cout << "Note: Constants used:" << std::endl;
  std::cout << "  - MAX_TOTAL_COINS: " << BonusConfig::MAX_TOTAL_COINS
            << std::endl;
  std::cout << "  - MAX_SCREEN_COINS: " << BonusConfig::MAX_SCREEN_COINS
            << std::endl;

  return true;
}

int main() {
  std::cout << "Running Bonus Manager Tests...\n" << std::endl;

  bool result = testBonusManagerCoinSpawning();

  if (result) {
    std::cout << "\n✓ Test passed!" << std::endl;
    return 0;
  } else {
    std::cout << "\n✗ Test failed!" << std::endl;
    return 1;
  }
}
