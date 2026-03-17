#pragma once
#include "InputHandler.h"

#include <iostream>

void InputHandler::setCommand(FRKey key, std::unique_ptr<ICommand> command) {
  keys[key] = std::move(command);
  std::cerr << "🔧 [InputHandler] Key command registered" << std::endl;
}

void InputHandler::setMouseCommand(FRMouseButton button,
                                   std::unique_ptr<ICommand> command) {
  mouseButtons[button] = std::move(command);
  std::cerr << "🖱️  [InputHandler] Mouse command registered" << std::endl;
}

void InputHandler::handleKeyPressed(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    std::cerr << "⌨️  [InputHandler] Key pressed" << std::endl;
    it->second->execute(player);
  }
}

void InputHandler::handleKeyReleased(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    std::cerr << "⌨️  [InputHandler] Key released" << std::endl;
    it->second->undo(player);
  }
}

void InputHandler::handleMousePressed(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    std::cerr << "🖱️  [InputHandler] Mouse pressed" << std::endl;
    it->second->execute(player);
  }
}

void InputHandler::handleMouseReleased(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    std::cerr << "🖱️  [InputHandler] Mouse released" << std::endl;
    it->second->undo(player);
  }
}
