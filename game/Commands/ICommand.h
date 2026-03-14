#pragma once
#include "../Player.h"
class ICommand {
public:
    virtual void execute(Player & player) = 0;
    virtual void undo(Player & player) = 0;
    virtual ~ICommand(){}
};
