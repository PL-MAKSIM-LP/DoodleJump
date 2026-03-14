#include "EnemyManager.h"
#include <cstdlib>
#include <algorithm>

static const int   MAX_ENEMIES   = 3;
static const float SPAWN_INTERVAL = 600.0f; // world units between enemies

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {
    if (sprite) destroySprite(sprite);
}

void EnemyManager::init(int w, int h) {
    screenWidth  = w;
    screenHeight = h;

    sprite = createSprite("../data/bottom-bug-tiles.png");
    setSpriteSize(sprite, 50, 40);
}

void EnemyManager::spawnEnemy(float y) {
    Enemy e;
    e.x = (float)(rand() % (screenWidth - e.w));
    e.y = y;
    enemies.push_back(e);
}

void EnemyManager::update(float dt, int sw, const Camera& camera) {
    for (auto& e : enemies)
        e.update(dt, sw);

    // Remove dead or off-screen enemies
    float bottomEdge = camera.offsetY + screenHeight + 100.0f;
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [bottomEdge](const Enemy& e) {
                return !e.alive || e.y > bottomEdge;
            }),
        enemies.end()
    );

    // Spawn new enemies ahead of the camera
    if ((int)enemies.size() < MAX_ENEMIES) {
        float spawnY = camera.offsetY - (float)(rand() % (int)SPAWN_INTERVAL) - 100.0f;
        spawnEnemy(spawnY);
    }
}

void EnemyManager::render(const Camera& camera) {
    for (auto& e : enemies)
        e.render(camera, sprite);
}

bool EnemyManager::checkPlayerCollision(const Player& player) {
    for (auto& e : enemies) {
        if (!e.alive) continue;
        bool overlapX = player.right() > e.left() && player.left() < e.right();
        bool overlapY = player.bottom() > e.top() && player.top() < e.bottom();
        if (overlapX && overlapY) return true;
    }
    return false;
}

bool EnemyManager::checkProjectileCollision(float px, float py, float pw, float ph) {
    for (auto& e : enemies) {
        if (!e.alive) continue;
        bool overlapX = px + pw > e.left() && px < e.right();
        bool overlapY = py + ph > e.top() && py < e.bottom();
        if (overlapX && overlapY) {
            e.alive = false;
            return true;
        }
    }
    return false;
}
