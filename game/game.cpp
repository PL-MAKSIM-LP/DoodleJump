#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../framework/Framework.h"
#include "core/Constants.h"
#include "core/GameEngine.h"

class MyFramework : public Framework {
  int screenWidth;
  int screenHeight;

  Sprite* background = nullptr;
  GameEngine gameEngine{screenWidth, screenHeight};

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
    std::cerr << "=== 🎮 MyFramework::Init() ===" << std::endl;
    background = createSprite("../sprites/jungle-bck@2x.png");
    if (background) {
      setSpriteSize(background, screenWidth, screenHeight);
      std::cerr << "✅ Background loaded" << std::endl;
    } else {
      std::cerr << "⚠️  Background not found" << std::endl;
    }

    gameEngine.init();
    std::cerr << "=== ✅ Framework initialized ===" << std::endl;

    return true;
  }

  virtual void Close() {
    if (background) destroySprite(background);
  }

  virtual bool Tick() {
    unsigned int now = getTickCount();
    float dt = (now - lastTick) / 1000.0f;
    lastTick = now;

    // Всегда рисуем фон
    drawSprite(background, 0, 0);

    // Обновляем и рендерим игру через GameEngine
    gameEngine.update(dt);
    gameEngine.render();

    return false;
  }

  virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {}

  virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
    if (gameEngine.getState() == GameState::PLAYING) {
      if (isReleased) {
        gameEngine.getInputHandler().handleMouseReleased(
            button, gameEngine.getPlayer());
      } else {
        gameEngine.getInputHandler().handleMousePressed(button,
                                                        gameEngine.getPlayer());
      }
    }
  }

  virtual void onKeyPressed(FRKey k) {
    if (gameEngine.getState() == GameState::PLAYING) {
      gameEngine.getInputHandler().handleKeyPressed(k, gameEngine.getPlayer());
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
