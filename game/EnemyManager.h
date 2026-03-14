#pragma once

#include "../framework/Framework.h"
#include "Enemy.h"
#include "Camera.h"
#include "Player.h"
#include <vector>

class EnemyManager {
public:
    EnemyManager();
    ~EnemyManager();

    void init(int screenWidth, int screenHeight);
    void update(float dt, int screenWidth, const Camera& camera);
    void render(const Camera& camera);

    // Returns true if player was hit by an enemy
    bool checkPlayerCollision(const Player& player);

    // Marks enemy as dead if hit by a projectile AABB
    bool checkProjectileCollision(float px, float py, float pw, float ph);

    std::vector<Enemy>& getEnemies() { return enemies; }

private:
    std::vector<Enemy> enemies;
    Sprite* sprite = nullptr;

    int screenWidth  = 0;
    int screenHeight = 0;

    void spawnEnemy(float y);
};
