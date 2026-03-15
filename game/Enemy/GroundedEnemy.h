#include "IEnemy.h"

class GroundedEnemy : public IEnemy {
  virtual void update(float dt, int screenWidth) override;
};