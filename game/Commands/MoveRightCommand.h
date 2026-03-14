#pragma once
#include "ICommand.h"

class MoveRightCommand : public ICommand {
 public:
  virtual void execute(Player& player) override { player.startMoveRight(); };

  virtual void undo(Player& player) override { player.stopMoveRight(); };
};
