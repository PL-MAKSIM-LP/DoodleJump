#include "Camera.h"

#include <iostream>

#include "core/Constants.h"

void Camera::update(float playerY, int screenHeight) {
  float desiredScreenY = screenHeight * CameraConfig::DESIRED_SCREEN_Y_FRAC;
  float curScreenY = playerY - offsetY;
  if (curScreenY < desiredScreenY) {
    offsetY = playerY - desiredScreenY;
    std::cerr << "[Camera::update] offsetY: " << " -> " << offsetY << std::endl;
  }
}

float Camera::toScreenY(float worldY) const { return worldY - offsetY; }
