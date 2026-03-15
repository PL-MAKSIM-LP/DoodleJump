#pragma once

#include "Platform.h"

class NormalPlatform : public IPlatform {
 public:
  NormalPlatform() = default;
  ~NormalPlatform() override = default;

  void update(float dt, int screenWidth) override;
};
