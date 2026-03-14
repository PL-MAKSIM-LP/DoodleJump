#include "Camera.h"

void Camera::update(float playerY, int screenHeight) {
    // Scroll up when player reaches the upper third of the screen
    float threshold = screenHeight / 3.0f;
    float screenPlayerY = playerY - offsetY;
    if (screenPlayerY < threshold) {
        offsetY -= threshold - screenPlayerY;
    }
}

float Camera::toScreenY(float worldY) const {
    return worldY - offsetY;
}
