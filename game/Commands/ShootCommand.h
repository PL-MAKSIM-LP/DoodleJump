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
  virtual void execute(Player& player) override {
    if (!mgr || !emgr) return;  // ИСПРАВКА: проверка emgr
    float sx = player.x + player.width / 2.0f - (ProjectileConfig::W / 2.0f);
    float sy = player.y;

    IEnemy* target = nullptr;
    float bestDist = 1e9f;
    for (auto& up : emgr->getEnemies()) {
      auto& e = up;
      if (!e || !e->isAlive()) continue;
      // consider enemy "on screen" if within player's screenHeight
      if (std::fabs(e->getY() - player.y) > player.screenHeight) continue;
      float ex = e->getX() + e->getW() / 2.0f;
      float ey = e->getY() + e->getH() / 2.0f;
      float dx = ex - sx;
      float dy = ey - sy;
      float d2 = dx * dx + dy * dy;
      if (d2 < bestDist) {
        bestDist = d2;
        target = e.get();
      }
    }

    float speed = ProjectileConfig::SHOOT_SPEED;
    float vx = 0.0f, vy = -speed;
    if (target) {
      float ex = target->getX() + target->getW() / 2.0f;
      float ey = target->getY() + target->getH() / 2.0f;
      float dx = ex - sx;
      float dy = ey - sy;
      float len = std::sqrt(dx * dx + dy * dy);
      if (len > 0.001f) {
        vx = dx / len * speed;
        vy = dy / len * speed;
      }
    }

    mgr->shoot(sx, sy, vx, vy);
  }
  virtual void undo(Player& player) override {}

 private:
  ProjectileManager* mgr = nullptr;
  EnemyManager* emgr = nullptr;
};
