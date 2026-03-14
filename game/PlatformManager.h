#pragma once

#include "../framework/Framework.h"
#include "Platform.h"
#include "Camera.h"
#include "Player.h"
#include <vector>

class PlatformManager {
public:
    PlatformManager();
    ~PlatformManager();

    void init(int screenWidth, int screenHeight);
    void update(float dt, int screenWidth, const Camera& camera);
    void render(const Camera& camera);

    bool checkCollision(Player& player);

    const std::vector<Platform>& getPlatforms() const { return platforms; }

private:
    std::vector<Platform> platforms;
    Sprite* sprite = nullptr;

    int screenWidth  = 0;
    int screenHeight = 0;

    void spawnPlatform(float y);
    void fillInitial();
};
