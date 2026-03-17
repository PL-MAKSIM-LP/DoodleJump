#pragma once
#include <map>
#include <memory>

#include "ICommand.h"

class InputHandler {
  std::map<FRKey, std::unique_ptr<ICommand>> keys;
  std::map<FRMouseButton, std::unique_ptr<ICommand>> mouseButtons;
  float lastMouseX = 0.0f;
  float lastMouseY = 0.0f;

 public:
  void handleKeyPressed(FRKey key, Player& player);
  void handleKeyReleased(FRKey key, Player& player);
  void handleMousePressed(FRMouseButton button, Player& player);
  void handleMouseReleased(FRMouseButton button, Player& player);

  void setCommand(FRKey key, std::unique_ptr<ICommand> command);
  void setMouseCommand(FRMouseButton button, std::unique_ptr<ICommand> command);

  void updateMousePosition(float x, float y) {
    lastMouseX = x;
    lastMouseY = y;
  }

  void clearAllCommands() {
    keys.clear();
    mouseButtons.clear();
  }
};
