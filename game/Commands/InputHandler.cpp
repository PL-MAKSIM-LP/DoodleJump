#include "InputHandler.h"

#include <iostream>

void InputHandler::setCommand(FRKey key, std::unique_ptr<ICommand> command) {
  std::cerr << "[InputHandler FRKey] set command" << std::endl;
  keys[key] = std::move(command);
}

void InputHandler::setMouseCommand(FRMouseButton button,
                                   std::unique_ptr<ICommand> command) {
  std::cerr << "[InputHandler FRMouseButton] set command" << std::endl;
  mouseButtons[button] = std::move(command);
}

void InputHandler::handleKeyPressed(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    std::cerr << "[InputHandler FRKey] Key pressed" << std::endl;
    it->second->execute(player);
  }
}

void InputHandler::handleKeyReleased(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    std::cerr << "[InputHandler FRKey] Key released" << std::endl;
    it->second->undo(player);
  }
}

void InputHandler::handleMousePressed(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    std::cerr << "[InputHandler FRMouseButton] Mouse pressed" << std::endl;
    it->second->execute(player, lastMouseX, lastMouseY);
  }
}

void InputHandler::handleMouseReleased(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    std::cerr << "[InputHandler FRMouseButton] Mouse released" << std::endl;
    it->second->undo(player);
  }
}