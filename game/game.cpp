#include "../framework/Framework.h"
#include "Camera.h"
#include "Commands/InputHandler.h"
#include "Commands/MoveLeftCommand.h"
#include "Commands/MoveRightCommand.h"
#include "EnemyManager.h"
#include "GameState.h"
#include "PlatformManager.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "ScoreDisplay.h"

class MyFramework : public Framework {
  int screenWidth;
  int screenHeight;

  GameState state = GameState::MENU;

  Sprite* background = nullptr;
  InputHandler inputHandler;
  Camera camera;
  Player player;
  PlatformManager platformManager;
  EnemyManager enemyManager;
  ProjectileManager projectileManager;
  ScoreDisplay scoreDisplay;

  unsigned int lastTick = 0;

 public:
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

    return true;
  }

  virtual void Close() { destroySprite(background); }

  virtual bool Tick() {
    drawSprite(background, 0, 0);
    player.render(camera);
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

int main(int argc, char* argv[]) { return run(new MyFramework); }
