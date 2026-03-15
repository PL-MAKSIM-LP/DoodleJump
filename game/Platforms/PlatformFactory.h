#pragma once

#include <memory>

#include "Platform.h"

class PlatformFactory {
 public:
  PlatformFactory() = default;
  ~PlatformFactory() = default;

  std::unique_ptr<IPlatform> create(int x, int y, PlatformType type);
};
