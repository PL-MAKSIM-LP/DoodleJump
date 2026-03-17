#pragma once

#include <cmath>

#include "../Camera/Camera.h"
#include "../Enemy/EnemyManager.h"
#include "../projectile/ProjectileManager.h"
#include "ICommand.h"
#include "core/Constants.h"

class ShootCommand : public ICommand {
 public:
  explicit ShootCommand(ProjectileManager* mgr, Camera* cam = nullptr)
      : mgr(mgr), camera(cam) {}

  virtual void execute(Player& player) override {}

  void execute(Player& player, float targetX, float targetY) {
    std::cerr << "[ShootCommand::execute] screen targetX=" << targetX
              << ", screen targetY=" << targetY
              << ", mgr=" << (mgr ? "OK" : "NULL")
              << ", camera=" << (camera ? "OK" : "NULL") << std::endl;

    if (mgr) {
      float playerCenterX = player.x + player.width / 2.0f;
      float playerCenterY = player.y + player.height / 2.0f;

      // Преобразуем экранные координаты мыши в мировые
      float worldTargetY = camera ? camera->toWorldY(targetY) : targetY;

      std::cerr << "[ShootCommand] world targetX=" << targetX
                << ", world targetY=" << worldTargetY << std::endl;
      std::cerr << "[ShootCommand] player center: (" << playerCenterX << ", "
                << playerCenterY << ")" << std::endl;

      mgr->shoot(playerCenterX, playerCenterY, targetX, worldTargetY);
    } else {
      std::cerr << "[ShootCommand] ProjectileManager is NULL!" << std::endl;
    }
  }

  void setCamera(Camera* cam) { camera = cam; }

  virtual void undo(Player& player) override {}

 private:
  ProjectileManager* mgr = nullptr;
  Camera* camera = nullptr;
};
