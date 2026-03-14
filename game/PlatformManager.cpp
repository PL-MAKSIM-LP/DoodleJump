#include "PlatformManager.h"
#include <cstdlib>
#include <algorithm>

static const int   PLATFORM_COUNT  = 10;
static const float VERTICAL_GAP    = 80.0f;

PlatformManager::PlatformManager() {}

PlatformManager::~PlatformManager() {
    if (sprite) destroySprite(sprite);
}

void PlatformManager::init(int w, int h) {
    screenWidth  = w;
    screenHeight = h;

    sprite = createSprite("../data/game-tiles.png");
    setSpriteSize(sprite, 70, 18);

    fillInitial();
}

void PlatformManager::fillInitial() {
    platforms.clear();
    for (int i = 0; i < PLATFORM_COUNT; ++i) {
        float y = screenHeight - i * VERTICAL_GAP;
        spawnPlatform(y);
    }
}

void PlatformManager::spawnPlatform(float y) {
    Platform p;
    p.x = (float)(rand() % (screenWidth - p.w));
    p.y = y;

    int r = rand() % 10;
    if (r < 6)      p.type = PlatformType::NORMAL;
    else if (r < 8) p.type = PlatformType::MOVING;
    else            p.type = PlatformType::BREAKABLE;

    platforms.push_back(p);
}

void PlatformManager::update(float dt, int sw, const Camera& camera) {
    for (auto& p : platforms)
        p.update(dt, sw);

    // Remove platforms that scrolled below the screen
    float bottomEdge = camera.offsetY + screenHeight + 100.0f;
    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(),
            [bottomEdge](const Platform& p) { return p.y > bottomEdge; }),
        platforms.end()
    );

    // Spawn new platforms at the top to maintain count
    while ((int)platforms.size() < PLATFORM_COUNT) {
        float topY = camera.offsetY - VERTICAL_GAP;
        spawnPlatform(topY);
    }
}

void PlatformManager::render(const Camera& camera) {
    for (auto& p : platforms)
        p.render(camera, sprite);
}

bool PlatformManager::checkCollision(Player& player) {
    // Only trigger when player is falling downward
    if (player.velY <= 0) return false;

    for (auto& p : platforms) {
        if (!p.active) continue;

        bool overlapX = player.right() > p.left() && player.left() < p.right();
        bool overlapY = player.bottom() >= p.top() && player.bottom() <= p.bottom();

        if (overlapX && overlapY) {
            if (p.type == PlatformType::BREAKABLE)
                p.active = false;
            else
                player.jump();
            return true;
        }
    }
    return false;
}
