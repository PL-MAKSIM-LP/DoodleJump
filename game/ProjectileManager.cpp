#include "ProjectileManager.h"
#include <algorithm>

ProjectileManager::ProjectileManager() {}

ProjectileManager::~ProjectileManager() {
    if (sprite) destroySprite(sprite);
}

void ProjectileManager::init() {
    sprite = createSprite("../data/projectile-tiles.png");
    setSpriteSize(sprite, 10, 20);
}

void ProjectileManager::shoot(float x, float y) {
    Projectile p;
    p.x = x;
    p.y = y;
    projectiles.push_back(p);
}

void ProjectileManager::update(float dt, const Camera& camera) {
    for (auto& p : projectiles)
        p.update(dt);

    // Remove projectiles that went off the top of the visible area
    float topEdge = camera.offsetY - 50.0f;
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [topEdge](const Projectile& p) {
                return !p.active || p.y < topEdge;
            }),
        projectiles.end()
    );
}

void ProjectileManager::render(const Camera& camera) {
    for (auto& p : projectiles)
        p.render(camera, sprite);
}
