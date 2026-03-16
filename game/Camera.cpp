#include "Camera.h"

#include "Constants.h"

void Camera::update(float playerY, int screenHeight) {
  float desiredScreenY = screenHeight * CameraConfig::DESIRED_SCREEN_Y_FRAC;
  float curScreenY = playerY - offsetY;
  if (curScreenY < desiredScreenY) {
    offsetY = playerY - desiredScreenY;
  }
}

float Camera::toScreenY(float worldY) const { return worldY - offsetY; }
