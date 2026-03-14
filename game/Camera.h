#pragma once

class Camera {
public:
    float offsetY = 0.0f;  // world units scrolled upward

    void update(float playerY, int screenHeight);

    // Convert world Y to screen Y
    float toScreenY(float worldY) const;
};
