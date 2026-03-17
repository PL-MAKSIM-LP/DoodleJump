#include "Camera.h"

#include <iostream>

#include "core/Constants.h"

void Camera::update(float playerY, int screenHeight) {
  float desiredScreenY = screenHeight * CameraConfig::DESIRED_SCREEN_Y_FRAC;
  float curScreenY = playerY - offsetY;
  if (curScreenY < desiredScreenY) {
    float oldOffset = offsetY;
    offsetY = playerY - desiredScreenY;
    std::cerr << "🔍 [Camera::update] offsetY: " << oldOffset << " -> "
              << offsetY << " (playerY=" << playerY << ")" << std::endl;
  }
}

float Camera::toScreenY(float worldY) const {
  return worldY - offsetY;  // No logging - called every frame for every entity
}
