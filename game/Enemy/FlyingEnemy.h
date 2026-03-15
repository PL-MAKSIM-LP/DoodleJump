#include "IEnemy.h"

class FlyingEnemy : public IEnemy {
  virtual void update(float dt, int screenWidth) override;
};