#pragma once

#include <random>

#include "../Bonuses/BonusManager.h"
#include "../Camera/Camera.h"
#include "../Commands/InputHandler.h"
#include "../Enemy/EnemyManager.h"
#include "../Platforms/PlatformManager.h"
#include "../Player/Player.h"
#include "../Score/ScoreDisplay.h"
#include "../core/GameState.h"
#include "../projectile/ProjectileManager.h"

class GameEngine {
 public:
  GameEngine(int screenWidth, int screenHeight);
  ~GameEngine();

  // Инициализация всех систем
  void init();

  // Основной игровой цикл
  void update(float dt);
  void render();

  // Управление состоянием
  GameState getState() const { return state; }
  void setState(GameState newState) { state = newState; }

  // Доступ к компонентам
  Player& getPlayer() { return player; }
  Camera& getCamera() { return camera; }
  InputHandler& getInputHandler() { return inputHandler; }
  PlatformManager& getPlatformManager() { return platformManager; }
  EnemyManager& getEnemyManager() { return enemyManager; }
  ProjectileManager& getProjectileManager() { return projectileManager; }
  BonusManager& getBonusManager() { return bonusManager; }
  ScoreDisplay& getScoreDisplay() { return scoreDisplay; }

  // Сброс игры
  void resetGame();

 private:
  int screenWidth;
  int screenHeight;

  GameState state = GameState::MENU;

  // Random number generator (used by all game systems)
  std::mt19937 rng;

  // Менеджеры всех систем
  PlatformManager platformManager;
  BonusManager bonusManager;
  InputHandler inputHandler;
  Camera camera;
  Player player;
  EnemyManager enemyManager;
  ProjectileManager projectileManager;
  ScoreDisplay scoreDisplay;

  // Игровой цикл по состояниям
  void updateMenu();
  void updatePlaying(float dt);
  void updateGameOver();

  void renderMenu();
  void renderPlaying();
  void renderGameOver();

  void renderGameWorld();  // Общий рендер объектов
};
