#pragma once
#include "ICommand.h"

class MoveLeftCommand : public ICommand {
 public:
  virtual void execute(Player& player) override { player.startMoveLeft(); };

  virtual void undo(Player& player) override { player.stopMoveLeft(); };
};
