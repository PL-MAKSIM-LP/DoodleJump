#include "GameEngine.h"

#include <ctime>
#include <iostream>

#include "../../framework/Framework.h"
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

GameEngine::~GameEngine() {
  platformManager.clear();
  enemyManager.clear();
  bonusManager.clear();
  projectileManager.clear();
  if (gameOverSprite) destroySprite(gameOverSprite);
}

void GameEngine::init() {
  auto shootCmd = std::make_unique<ShootCommand>(&projectileManager, &camera);
  inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
  inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
  inputHandler.setMouseCommand(FRMouseButton::LEFT, std::move(shootCmd));
  inputHandler.setMouseCommand(
      FRMouseButton::RIGHT,
      std::make_unique<SpawnTrampolineCommand>(&bonusManager));

  scoreDisplay.init();
  platformManager.init(screenWidth, screenHeight);
  platformManager.setRandomGenerator(rng);
  enemyManager.init(screenWidth, screenHeight);
  enemyManager.setPlatformManager(&platformManager);
  enemyManager.setRandomGenerator(rng);
  bonusManager.setRandomGenerator(rng);
  projectileManager.init();

  gameOverSprite = createSprite(DebugConfig::GAME_OVER_SPRITE_PATH);
  if (!gameOverSprite) {
    std::cerr << "[GameEngine::init()] Failed to load game over sprite"
              << std::endl;
    createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
  }

  setSpriteSize(gameOverSprite, 200, 50);

  state = GameState::MENU;
}

void GameEngine::resetGame() {
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

  state = GameState::PLAYING;
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
  player.update(dt, screenWidth, camera);
  platformManager.update(dt, player, scoreDisplay);
  enemyManager.update(dt, screenWidth, camera, player.y);
  projectileManager.update(dt, camera);
  bonusManager.update(screenWidth, screenHeight, camera, player);

  EnemyCollision collisionResult = enemyManager.checkPlayerCollision(player);
  if (collisionResult == EnemyCollision::HIT) {
    std::cerr << "[GameEngine::updatePlaying()] GAME OVER" << std::endl;
    state = GameState::GAME_OVER;
    return;
  } else if (collisionResult == EnemyCollision::STOMP) {
    std::cerr << "[GameEngine::updatePlaying()] Player stomped on enemy"
              << std::endl;
    player.jump();
  }

  if (!player.alive) {
    std::cerr << "[GameEngine::updatePlaying()] GAME OVER: Player died (fell "
                 "off screen)"
              << std::endl;
    state = GameState::GAME_OVER;
    return;
  }
  camera.update(player.y, screenHeight);

  for (auto& pr : projectileManager.getProjectiles()) {
    if (!pr.active) continue;
    Object projectileObj{pr.left(), pr.top(), pr.right() - pr.left(),
                         pr.bottom() - pr.top()};

    if (enemyManager.checkProjectileCollision(projectileObj)) {
      pr.active = false;
    }
  }
}

void GameEngine::updateGameOver() { camera.update(player.y, screenHeight); }

void GameEngine::restartGame() {
  resetGame();
}

void GameEngine::render() {
  switch (state) {
    case GameState::MENU:
      break;
    case GameState::PLAYING:
      renderGame();
      break;
    case GameState::GAME_OVER:
      renderGame();
      if (gameOverSprite) {
        drawSprite(gameOverSprite, (screenWidth - 200) / 2,
                   (screenHeight - 50) / 2);
      }
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
