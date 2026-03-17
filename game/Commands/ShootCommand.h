#pragma once

#include <cmath>

#include "../Enemy/EnemyManager.h"
#include "../projectile/ProjectileManager.h"
#include "ICommand.h"
#include "core/Constants.h"

class ShootCommand : public ICommand {
 public:
  explicit ShootCommand(ProjectileManager* mgr, EnemyManager* emgr)
      : mgr(mgr), emgr(emgr) {}
  virtual void execute(Player& player) override {}
  virtual void undo(Player& player) override {}

 private:
  ProjectileManager* mgr = nullptr;
  EnemyManager* emgr = nullptr;
};
