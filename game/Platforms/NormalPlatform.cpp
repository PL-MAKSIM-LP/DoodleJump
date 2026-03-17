#include "NormalPlatform.h"

#include <iostream>

// Empty implementation for now
// This is a basic platform with no special behavior
// Uses default IPlatform::render() and no special logic
NormalPlatform::NormalPlatform() {
  std::cerr << "🟢 [NormalPlatform::constructor] Created" << std::endl;
}
