#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../framework/Framework.h"
#include "Camera.h"
#include "Commands/InputHandler.h"
#include "Commands/MoveLeftCommand.h"
#include "Commands/MoveRightCommand.h"
#include "Enemy/EnemyManager.h"
#include "GameState.h"
#include "Platforms/Platform.h"
#include "Platforms/PlatformManager.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "ScoreDisplay.h"

class MyFramework : public Framework {
  int screenWidth = 390;
  int screenHeight = 844;

  GameState state = GameState::MENU;
  PlatformManager platformManager;

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
    setSpriteSize(background, screenWidth, screenHeight);
    player.init(screenWidth, screenHeight);

    inputHandler.setCommand(FRKey::LEFT, std::make_unique<MoveLeftCommand>());
    inputHandler.setCommand(FRKey::RIGHT, std::make_unique<MoveRightCommand>());
    platformManager.init(screenWidth, screenHeight);
    enemyManager.init(screenWidth, screenHeight);

    player.jump();

    return true;
  }

  virtual void Close() { destroySprite(background); }

  virtual bool Tick() {
    unsigned int now = getTickCount();
    float dt = (now - lastTick) / 1000.0f;
    lastTick = now;

    drawSprite(background, 0, 0);
    player.update(dt, screenWidth);
    platformManager.update(dt, player);
    enemyManager.update(dt, screenWidth, camera, player.y);

    // check collision with enemies
    if (enemyManager.checkPlayerCollision(player)) {
      std::cout << "Player collided with enemy" << std::endl;
    }

    // update camera after game objects updated, then render using camera
    camera.update(player.y, screenHeight);
    player.render(camera);
    platformManager.render(camera);
    enemyManager.render(camera);

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
  int w = 390, h = 844;
  if (argc == 3) {
    try {
      w = std::stoi(argv[1]);
      h = std::stoi(argv[2]);
    } catch (const std::exception& e) {
      std::cout << "Invalid arguments, using defaults: " << e.what()
                << std::endl;
      w = 390;
      h = 844;
    }
  }

  if (w < 0 || w > 1920 || h < 0 || h > 1080) {
    std::cout << "Invalid arguments, using defaults: width and height should "
                 "be between 0 and 2000"
              << std::endl;
    w = 390;
    h = 844;
  }

  return run(new MyFramework(w, h));
}
