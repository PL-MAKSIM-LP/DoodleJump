#include "Camera.h"

#include "Constants.h"

void Camera::update(float playerY, int screenHeight) {
  // Keep the player at about 40% from the top of the screen when moving up.
  float desiredScreenY = screenHeight * CameraConfig::DESIRED_SCREEN_Y_FRAC;

  // current player screen Y = worldY - offsetY
  float curScreenY = playerY - offsetY;

  // If the player moved above the desired position (smaller screen Y), move
  // camera up.
  if (curScreenY < desiredScreenY) {
    offsetY = playerY - desiredScreenY;
  }
  // Do not move camera downwards; keep the highest reached offset.
}

float Camera::toScreenY(float worldY) const { return worldY - offsetY; }
