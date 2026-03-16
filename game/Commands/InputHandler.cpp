#pragma once
#include "InputHandler.h"

void InputHandler::setCommand(FRKey key, std::unique_ptr<ICommand> command) {
  keys[key] = std::move(command);
}

void InputHandler::setMouseCommand(FRMouseButton button,
                                   std::unique_ptr<ICommand> command) {
  mouseButtons[button] = std::move(command);
}

void InputHandler::handleKeyPressed(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    it->second->execute(player);
  }
}

void InputHandler::handleKeyReleased(FRKey key, Player& player) {
  auto it = keys.find(key);
  if (it != keys.end()) {
    it->second->undo(player);
  }
}

void InputHandler::handleMousePressed(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    it->second->execute(player);
  }
}

void InputHandler::handleMouseReleased(FRMouseButton button, Player& player) {
  auto it = mouseButtons.find(button);
  if (it != mouseButtons.end()) {
    it->second->undo(player);
  }
}
