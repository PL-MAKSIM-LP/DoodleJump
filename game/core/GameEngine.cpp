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
    : screenWidth(width),
      screenHeight(height),
      rng(static_cast<unsigned int>(std::time(nullptr))) {}

GameEngine::~GameEngine() {}

void GameEngine::init() {
  std::cerr << "[GameEngine::init()] start" << std::endl;

  inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
  inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
  inputHandler.setMouseCommand(
      FRMouseButton::LEFT,
      std::make_unique<ShootCommand>(&projectileManager, &enemyManager));
  inputHandler.setMouseCommand(
      FRMouseButton::RIGHT,
      std::make_unique<SpawnTrampolineCommand>(&bonusManager));

  scoreDisplay.init();
  platformManager.init(screenWidth, screenHeight);
  platformManager.setRandomGenerator(rng);
  enemyManager.init(screenWidth, screenHeight);
  enemyManager.setPlatformManager(&platformManager);
  enemyManager.setRandomGenerator(rng);
  projectileManager.init();

  state = GameState::MENU;
  std::cerr << "[GameEngine::init()] end" << std::endl;
}

void GameEngine::resetGame() {
  std::cerr << "[GameEngine::resetGame()] start" << std::endl;
  platformManager.clear();
  platformManager.init(screenWidth, screenHeight);
  platformManager.setRandomGenerator(rng);

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

  inputHandler.clearAllCommands();

  inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
  inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
  inputHandler.setMouseCommand(
      FRMouseButton::LEFT,
      std::make_unique<ShootCommand>(&projectileManager, &enemyManager));
  inputHandler.setMouseCommand(
      FRMouseButton::RIGHT,
      std::make_unique<SpawnTrampolineCommand>(&bonusManager));

  state = GameState::PLAYING;
  std::cerr << "[GameEngine::resetGame()] end" << std::endl;
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
  resetGame();
  state = GameState::PLAYING;
}

void GameEngine::updatePlaying(float dt) {
    player.update(dt, screenWidth);
  platformManager.update(dt, player);
  enemyManager.update(dt, screenWidth, camera, player.y);
  projectileManager.update(dt, camera);
  bonusManager.update(screenWidth, screenHeight, camera, player);

  EnemyCollision collisionResult = enemyManager.checkPlayerCollision(player);
  if (collisionResult == EnemyCollision::HIT) {
    std::cerr << "[GameEngine::updatePlaying()] GAME OVER" << std::endl;
    // state = GameState::GAME_OVER;
    return;
  } else if (collisionResult == EnemyCollision::STOMP) {
    std::cerr << "[GameEngine::updatePlaying()] Player stomped on enemy"
              << std::endl;
    player.jump();
  }
  std::cerr << "[GameEngine::2]" << std::endl;
  if (!player.alive) {
    std::cerr << "[GameEngine::updatePlaying()] GAME OVER: Player died (fell "
                 "off screen)"
              << std::endl;
    state = GameState::GAME_OVER;
    return;
  }
  std::cerr << "[GameEngine::3]" << std::endl;
  camera.update(player.y, screenHeight);

  for (auto& pr : projectileManager.getProjectiles()) {
    if (!pr.active) continue;
    Object projectileObj{pr.left(), pr.top(), pr.right() - pr.left(),
                         pr.bottom() - pr.top()};

    if (enemyManager.checkProjectileCollision(projectileObj)) {
      pr.active = false;
    }
  }
  std::cerr << "[GameEngine::4]" << std::endl;
}

void GameEngine::updateGameOver() {
  // TODO: Добавить логику для экрана Game Over (например, ждать нажатия кнопки
  // для перезапуска)
  camera.update(player.y, screenHeight);
}

void GameEngine::render() {
  switch (state) {
    case GameState::MENU:
      // renderMenu();
      break;
    case GameState::PLAYING:
      renderGame();
      break;
    case GameState::GAME_OVER:
      // renderGameOver();
      break;
  }
}

void GameEngine::renderGame() {
  player.render(camera);
  platformManager.render(camera);
  enemyManager.render(camera);
  projectileManager.render(camera);
  bonusManager.render(camera);

  scoreDisplay.setScore(GameData::getCoins());
  scoreDisplay.render();
}
