#pragma once

#include "Platform.h"

class BreakablePlatform : public IPlatform {
 public:
  BreakablePlatform() = default;
  ~BreakablePlatform() override = default;

  void update(float dt, int screenWidth) override;
  void onCollide(Player& player) override;

 private:
  bool broken = false;
};
