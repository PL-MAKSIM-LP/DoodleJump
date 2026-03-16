#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../framework/Framework.h"
#include "Bonuses/BonusManager.h"
#include "Camera.h"
#include "Commands/InputHandler.h"
#include "Commands/MoveLeftCommand.h"
#include "Commands/MoveRightCommand.h"
#include "Commands/ShootCommand.h"
#include "Commands/SpawnTrampolineCommand.h"
#include "Constants.h"
#include "Enemy/EnemyManager.h"
#include "GameState.h"
#include "Platforms/Platform.h"
#include "Platforms/PlatformManager.h"
#include "Player.h"
#include "ScoreDisplay.h"
#include "projectile/ProjectileManager.h"

class MyFramework : public Framework {
  int screenWidth = GameConfig::DEFAULT_WIDTH;
  int screenHeight = GameConfig::DEFAULT_HEIGHT;

  GameState state = GameState::MENU;
  PlatformManager platformManager;
  BonusManager bonusManager;
  Sprite* background = nullptr;
  InputHandler inputHandler;
  Camera camera;
  Player player;
  EnemyManager enemyManager;
  ProjectileManager projectileManager;
  ScoreDisplay scoreDisplay;

  unsigned int lastTick = 0;

 public:
  MyFramework(int width, int height) {
    screenWidth = width;
    screenHeight = height;
  }

  virtual void PreInit(int& width, int& height, bool& fullscreen) override {
    width = screenWidth;
    height = screenHeight;
    fullscreen = false;
  }

  virtual bool Init() {
    background = createSprite("../data/jungle-bck@2x.png");
    if (background) setSpriteSize(background, screenWidth, screenHeight);
    player.init(screenWidth, screenHeight);

    inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
    inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
    inputHandler.setMouseCommand(
        FRMouseButton::LEFT,
        std::make_unique<ShootCommand>(&projectileManager, &enemyManager));

    inputHandler.setMouseCommand(
        FRMouseButton::RIGHT,
        std::make_unique<SpawnTrampolineCommand>(&bonusManager));

    platformManager.init(screenWidth, screenHeight);
    enemyManager.init(screenWidth, screenHeight);
    enemyManager.setPlatformManager(&platformManager);
    projectileManager.init();

    player.jump();

    return true;
  }

  virtual void Close() {
    if (background) destroySprite(background);
  }

  virtual bool Tick() {
    unsigned int now = getTickCount();
    float dt = (now - lastTick) / 1000.0f;
    lastTick = now;

    drawSprite(background, 0, 0);
    player.update(dt, screenWidth);
    platformManager.update(dt, player);

    enemyManager.update(dt, screenWidth, camera, player.y);
    projectileManager.update(dt, camera);

    if (enemyManager.checkPlayerCollision(player)) {
      std::cout << "Player collided with enemy" << std::endl;
    }

    // update camera after game objects updated, then render using camera
    camera.update(player.y, screenHeight);
    player.render(camera);
    platformManager.render(camera);
    enemyManager.render(camera);
    // check projectile collisions with enemies
    for (auto& pr : projectileManager.getProjectiles()) {
      if (!pr.active) continue;
      if (enemyManager.checkProjectileCollision(pr.left(), pr.top(),
                                                pr.right() - pr.left(),
                                                pr.bottom() - pr.top())) {
        pr.active = false;
      }
    }
    projectileManager.render(camera);

    return false;
  }

  virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {}

  virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {}

  virtual void onKeyPressed(FRKey k) {
    inputHandler.handleKeyPressed(k, player);
  }

  virtual void onKeyReleased(FRKey k) {
    inputHandler.handleKeyReleased(k, player);
  }

  virtual const char* GetTitle() override { return "DoodleJump"; }
};

int main(int argc, char* argv[]) {
  int w = GameConfig::DEFAULT_WIDTH, h = GameConfig::DEFAULT_HEIGHT;
  if (argc == 3) {
    try {
      w = std::stoi(argv[1]);
      h = std::stoi(argv[2]);
    } catch (const std::exception& e) {
      std::cout << "Invalid arguments, using defaults: " << e.what()
                << std::endl;
      w = GameConfig::DEFAULT_WIDTH;
      h = GameConfig::DEFAULT_HEIGHT;
    }
  }

  if (w < 0 || w > GameConfig::MAX_WIDTH || h < 0 ||
      h > GameConfig::MAX_HEIGHT) {
    std::cout << "Invalid arguments, using defaults: width and height should "
                 "be between 0 and 2000"
              << std::endl;
    w = GameConfig::DEFAULT_WIDTH;
    h = GameConfig::DEFAULT_HEIGHT;
  }

  return run(new MyFramework(w, h));
}
