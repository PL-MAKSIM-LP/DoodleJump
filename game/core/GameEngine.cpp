#include "GameEngine.h"

#include <ctime>
#include <iostream>

#include "../Commands/MoveLeftCommand.h"
#include "../Commands/MoveRightCommand.h"
#include "../Commands/ShootCommand.h"
#include "../Commands/SpawnTrampolineCommand.h"
#include "../Enemy/EnemyManager.h"
#include "../GameData/GameData.h"
#include "../Platforms/BreakablePlatform.h"

GameEngine::GameEngine(int width, int height)
    : screenWidth(width), screenHeight(height),
      rng(static_cast<unsigned int>(std::time(nullptr))) {}

GameEngine::~GameEngine() {}

void GameEngine::init() {
  std::cerr << "=== 🟢 GameEngine::init() ===" << std::endl;
  // Инициализируем input команды
  inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
  inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
  inputHandler.setMouseCommand(
      FRMouseButton::LEFT,
      std::make_unique<ShootCommand>(&projectileManager, &enemyManager));
  inputHandler.setMouseCommand(
      FRMouseButton::RIGHT,
      std::make_unique<SpawnTrampolineCommand>(&bonusManager));

  // Инициализируем все менеджеры
  scoreDisplay.init();
  platformManager.init(screenWidth, screenHeight);
  enemyManager.init(screenWidth, screenHeight);
  enemyManager.setPlatformManager(&platformManager);
  enemyManager.setRandomGenerator(rng);
  projectileManager.init();

  // Начинаем с меню
  state = GameState::MENU;
  std::cerr << "=== ✅ GameEngine initialized ===" << std::endl;
}

void GameEngine::resetGame() {
  std::cerr << "=== 🔄 GameEngine::resetGame() ===" << std::endl;
  platformManager.clear();
  platformManager.init(screenWidth, screenHeight);

  bonusManager.clear();

  enemyManager.clear();
  enemyManager.init(screenWidth, screenHeight);
  enemyManager.setRandomGenerator(rng);

  projectileManager.clear();

  camera = Camera();

  player = Player();
  player.init(screenWidth, screenHeight);
  player.jump();

  scoreDisplay = ScoreDisplay();
  scoreDisplay.init();
  GameData::resetCoins();

  // ИСПРАВКА: перевсоздаём команды с актуальными указателями на менеджеры
  inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
  inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
  inputHandler.setMouseCommand(
      FRMouseButton::LEFT,
      std::make_unique<ShootCommand>(&projectileManager, &enemyManager));
  inputHandler.setMouseCommand(
      FRMouseButton::RIGHT,
      std::make_unique<SpawnTrampolineCommand>(&bonusManager));

  state = GameState::PLAYING;
  std::cerr << "=== ✅ Game reset and started ===" << std::endl;
}

void GameEngine::update(float dt) {
  switch (state) {
    case GameState::MENU:
      updateMenu();
      break;
    case GameState::PLAYING:
      updatePlaying(dt);
      break;
    case GameState::GAME_OVER:
      updateGameOver();
      break;
  }
}

void GameEngine::updateMenu() {
  // TODO: Добавить экран меню с кнопкой "START"
  // На данный момент сразу начинаем игру
  resetGame();
  state = GameState::PLAYING;
}

void GameEngine::updatePlaying(float dt) {
  // Обновляем игровые объекты
  player.update(dt, screenWidth);
  platformManager.update(dt, player);
  enemyManager.update(dt, screenWidth, camera, player.y);
  projectileManager.update(dt, camera);
  bonusManager.update(screenWidth, screenHeight, camera, player);

  // Проверяем столкновение с врагом
  EnemyCollision collisionResult = enemyManager.checkPlayerCollision(player);
  if (collisionResult == EnemyCollision::HIT) {
    std::cerr << "❌ GAME OVER: Player collided with enemy" << std::endl;
    // state = GameState::GAME_OVER;
    return;
  } else if (collisionResult == EnemyCollision::STOMP) {
    std::cerr << "✓ Player stomped on enemy" << std::endl;
    player.jump();
  }

  // Проверяем падение игрока
  if (!player.alive) {
    std::cerr << "❌ GAME OVER: Player died (fell off screen)" << std::endl;
    state = GameState::GAME_OVER;
    return;
  }

  // Обновляем камеру
  camera.update(player.y, screenHeight);

  // Проверяем столкновения снарядов с врагами
  for (auto& pr : projectileManager.getProjectiles()) {
    if (!pr.active) continue;
    if (enemyManager.checkProjectileCollision(pr.left(), pr.top(),
                                              pr.right() - pr.left(),
                                              pr.bottom() - pr.top())) {
      pr.active = false;
    }
  }
}

void GameEngine::updateGameOver() {
  // В состоянии игрок падает, обновляем камеру для финальной сцены
  camera.update(player.y, screenHeight);
}

void GameEngine::render() {
  switch (state) {
    case GameState::MENU:
      renderMenu();
      break;
    case GameState::PLAYING:
      renderPlaying();
      break;
    case GameState::GAME_OVER:
      renderGameOver();
      break;
  }
}

void GameEngine::renderMenu() {
  // TODO: Добавить экран меню
}

void GameEngine::renderGameWorld() {
  // Общий рендер все игровых объектов
  player.render(camera);
  platformManager.render(camera);
  enemyManager.render(camera);
  projectileManager.render(camera);
  bonusManager.render(camera);

  // Обновляем и отображаем счет монет
  scoreDisplay.setScore(GameData::getCoins());
  scoreDisplay.render();
}

void GameEngine::renderPlaying() { renderGameWorld(); }

void GameEngine::renderGameOver() {
  // Показываем финальную сцену
  renderGameWorld();

  // TODO: Добавить экран Game Over с счетом и кнопкой "RETRY"
}
