#pragma once
#include "InputHandler.h"

void InputHandler::setCommand(FRKey key, std::unique_ptr<ICommand> command) {
  keys[key] = std::move(command);
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
