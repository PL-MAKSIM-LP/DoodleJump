#include <vector>

#include "IEnemy.h"

class FlyingEnemy : public IEnemy {
 private:
  std::vector<Sprite*> flyFrames;
  int currentFrameIndex = 0;
  float animationTimer = 0.0f;
  float frameDuration = 0.12f;  // seconds between frames

 public:
  void setFlyFrames(const std::vector<Sprite*>& frames,
                    float duration = 0.12f) {
    flyFrames = frames;
    frameDuration = duration;
    currentFrameIndex = 0;
    if (!flyFrames.empty()) sprite = flyFrames[0];
  }

  virtual void update(float dt, int screenWidth) override;
};