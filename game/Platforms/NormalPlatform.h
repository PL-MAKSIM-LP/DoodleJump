#pragma once

#include "Platform.h"

class NormalPlatform : public IPlatform {
 public:
  NormalPlatform();
  ~NormalPlatform() override = default;

  void update(float dt, int screenHeight) override {}
};
