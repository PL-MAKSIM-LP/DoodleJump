#pragma once

#include "../framework/Framework.h"
#include "Projectile.h"
#include "Camera.h"
#include <vector>

class ProjectileManager {
public:
    ProjectileManager();
    ~ProjectileManager();

    void init();
    void update(float dt, const Camera& camera);
    void render(const Camera& camera);

    void shoot(float x, float y);

    std::vector<Projectile>& getProjectiles() { return projectiles; }

private:
    std::vector<Projectile> projectiles;
    Sprite* sprite = nullptr;
};
