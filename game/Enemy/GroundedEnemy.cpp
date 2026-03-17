#include "GroundedEnemy.h"

#include <iostream>

void GroundedEnemy::update(float dt, int screenWidth) {
  // TODO: Implement grounded enemy movement
  // For now, just empty - display warning once
  static bool warned = false;
  if (!warned) {
    std::cerr << "🟡 [GroundedEnemy::update] NOT IMPLEMENTED - enemy at x=" << x
              << std::endl;
    warned = true;
  }
}