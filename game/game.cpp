#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "../framework/Framework.h"
#include "Helpers/SpriteHelper.h"
#include "core/Constants.h"
#include "core/GameEngine.h"

struct MousePosition {
  int x, y;
};

class MyFramework : public Framework {
  int screenWidth;
  int screenHeight;

  Sprite* background = nullptr;
  GameEngine gameEngine{screenWidth, screenHeight};
  MousePosition mouse;

  unsigned int lastTick = 0;

 public:
  MyFramework(int width, int height)
      : screenWidth(width),
        screenHeight(height),
        gameEngine(screenWidth, screenHeight) {}

  virtual void PreInit(int& width, int& height, bool& fullscreen) override {
    width = screenWidth;
    height = screenHeight;
    fullscreen = false;
  }

  virtual bool Init() {
    SpriteHelper::BytesArrayToPNG();

    background = createSprite(BGConfig::BG_SPRITE_PATH);
    if (!background) {
      background = createSprite(DebugConfig::DEFAULT_SPRITE_PATH);
      std::cerr << "[MyFramework::Init()] fail to load background" << std::endl;
    }
    setSpriteSize(background, screenWidth, screenHeight);

    gameEngine.init();

    return true;
  }

  virtual void Close() {
    if (background) destroySprite(background);
  }

  virtual bool Tick() {
    unsigned int now = getTickCount();
    float dt = (now - lastTick) / 1000.0f;
    lastTick = now;

    if (background) {
      drawSprite(background, 0, 0);
    }

    gameEngine.update(dt);
    gameEngine.render();

    return false;
  }

  virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
    mouse.x = x;
    mouse.y = y;
    gameEngine.getInputHandler().updateMousePosition(static_cast<float>(x),
                                                     static_cast<float>(y));
  }

  virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
    if (gameEngine.getState() == GameState::PLAYING) {
      if (isReleased) {
        gameEngine.getInputHandler().handleMouseReleased(
            button, gameEngine.getPlayer());
      } else {
        gameEngine.getInputHandler().handleMousePressed(button,
                                                        gameEngine.getPlayer());
      }
    } else if (gameEngine.getState() == GameState::GAME_OVER) {
      gameEngine.restartGame();
    }
  }

  virtual void onKeyPressed(FRKey k) {
    if (gameEngine.getState() == GameState::PLAYING) {
      gameEngine.getInputHandler().handleKeyPressed(k, gameEngine.getPlayer());
    } else if (gameEngine.getState() == GameState::GAME_OVER) {
      gameEngine.restartGame();
    }
  }

  virtual void onKeyReleased(FRKey k) {
    if (gameEngine.getState() == GameState::PLAYING) {
      gameEngine.getInputHandler().handleKeyReleased(k, gameEngine.getPlayer());
    }
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

  std::cout << "Starting game with width=" << w << " height=" << h << std::endl;

  if (w < 0 || w > GameConfig::MAX_WIDTH || h < 0 ||
      h > GameConfig::MAX_HEIGHT) {
    std::cout << "Invalid arguments, width and height must be between 0 and "
              << GameConfig::MAX_WIDTH << " for width and 0 and "
              << GameConfig::MAX_HEIGHT << " for height." << std::endl;
    w = GameConfig::DEFAULT_WIDTH;
    h = GameConfig::DEFAULT_HEIGHT;
  }

  auto framework = std::make_unique<MyFramework>(w, h);
  return run(framework.get());
}
