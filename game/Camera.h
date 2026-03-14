#pragma once

class Camera {
public:
    float offsetY = 0.0f;

    void update(float playerY, int screenHeight);
    float toScreenY(float worldY) const;
};
