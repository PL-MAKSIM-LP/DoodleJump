#pragma once

#include <cmath>

#include "../Enemy/EnemyManager.h"
#include "../projectile/ProjectileManager.h"
#include "ICommand.h"

class ShootCommand : public ICommand {
 public:
  explicit ShootCommand(ProjectileManager* mgr, EnemyManager* emgr)
      : mgr(mgr), emgr(emgr) {}
  virtual void execute(Player& player) override {
    if (!mgr) return;
    float sx = player.x + player.width / 2.0f - 5.0f;
    float sy = player.y;

    // find nearest enemy on screen (relative to player)
    Enemy* target = nullptr;
    float bestDist = 1e9f;
    for (auto& e : emgr->getEnemies()) {
      // consider enemy "on screen" if within player's screenHeight
      if (std::fabs(e.y - player.y) > player.screenHeight) continue;
      float ex = e.x + e.w / 2.0f;
      float ey = e.y + e.h / 2.0f;
      float dx = ex - sx;
      float dy = ey - sy;
      float d2 = dx * dx + dy * dy;
      if (d2 < bestDist) {
        bestDist = d2;
        target = &e;
      }
    }

    float speed = 700.0f;
    float vx = 0.0f, vy = -speed;
    if (target) {
      float ex = target->x + target->w / 2.0f;
      float ey = target->y + target->h / 2.0f;
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
