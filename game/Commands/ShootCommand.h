#pragma once

#include <cmath>

#include "../Enemy/EnemyManager.h"
#include "../projectile/ProjectileManager.h"
#include "ICommand.h"
#include "core/Constants.h"

class ShootCommand : public ICommand {
 public:
  explicit ShootCommand(ProjectileManager* mgr) : mgr(mgr) {}

  virtual void execute(Player& player) override {
    if (mgr) {
      MousePosition pos = getMousePosition();
      mgr->shoot(pos);
    }
  }

  virtual void undo(Player& player) override {}

 private:
  ProjectileManager* mgr = nullptr;
};
