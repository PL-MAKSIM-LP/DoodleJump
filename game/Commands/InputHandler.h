#pragma once
#include <map>
#include <memory>

#include "ICommand.h"

class InputHandler {
  std::map<FRKey, std::unique_ptr<ICommand>> keys;

 public:
  void handleKeyPressed(FRKey key, Player& player);

  void handleKeyReleased(FRKey, Player&);
  void setCommand(FRKey key, std::unique_ptr<ICommand> command);
};
