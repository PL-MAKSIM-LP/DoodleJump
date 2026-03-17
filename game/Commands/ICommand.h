#pragma once
#include "../Player/Player.h"
class ICommand {
 public:
  virtual void execute(Player& player) = 0;
  virtual void execute(Player& player, float mouseX, float mouseY) {
    execute(player);  // по умолчанию игнорируем позицию мыши
  }
  virtual void undo(Player& player) = 0;
  virtual ~ICommand() {}
};
