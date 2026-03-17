#pragma once

#include <random>

#include "../Bonuses/BonusManager.h"
#include "../Camera/Camera.h"
#include "../Commands/InputHandler.h"
#include "../Enemy/EnemyManager.h"
#include "../Platforms/PlatformManager.h"
#include "../Player/Player.h"
#include "../Score/ScoreDisplay.h"
#include "../projectile/ProjectileManager.h"

enum class GameState { MENU, PLAYING, GAME_OVER };

class GameEngine {
 public:
  GameEngine(int screenWidth, int screenHeight);
  ~GameEngine();

  void init();

  void update(float dt);
  void render();

  GameState getState() const { return state; }
  void setState(GameState newState) { state = newState; }

  Player& getPlayer() { return player; }
  Camera& getCamera() { return camera; }
  InputHandler& getInputHandler() { return inputHandler; }
  PlatformManager& getPlatformManager() { return platformManager; }
  EnemyManager& getEnemyManager() { return enemyManager; }
  ProjectileManager& getProjectileManager() { return projectileManager; }
  BonusManager& getBonusManager() { return bonusManager; }
  ScoreDisplay& getScoreDisplay() { return scoreDisplay; }

  void resetGame();

 private:
  int screenWidth;
  int screenHeight;

  GameState state = GameState::MENU;

  std::mt19937 rng;

  PlatformManager platformManager;
  ProjectileManager projectileManager;
  BonusManager bonusManager;
  EnemyManager enemyManager;

  InputHandler inputHandler;
  Camera camera;
  Player player;
  ScoreDisplay scoreDisplay;

  void updateMenu();
  void updatePlaying(float dt);
  void updateGameOver();

  void renderPlaying();

  void renderGame();
};
