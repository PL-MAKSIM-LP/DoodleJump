#pragma once

class Camera {
 public:
  void update(float playerY, int screenHeight);
  float toScreenY(float worldY) const;
  float toWorldY(float screenY) const;

 private:
  float offsetY = 0.0f;
};
