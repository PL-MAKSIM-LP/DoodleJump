#pragma once

#include <vector>

#include "IEnemy.h"

class GroundedEnemy : public IEnemy {
 private:
  std::vector<Sprite*> walkFrames;
  int currentFrameIndex = 0;
  float animationTimer = 0.0f;
  float frameDuration = 0.15f;  // TODO
  float velocityX = 1.5f;

 public:
  void setWalkFrames(const std::vector<Sprite*>& frames,
                     float duration = 0.15f) {
    walkFrames = frames;
    frameDuration = duration;
    currentFrameIndex = 0;
  }

  virtual void update(float dt, int screenWidth) override;
};