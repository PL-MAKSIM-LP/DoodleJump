#include "EnemyManager.h"
#include <cstdlib>
#include <algorithm>

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {}

void EnemyManager::init(int w, int h) {}

void EnemyManager::update(float dt, int screenWidth, const Camera& camera) {}

void EnemyManager::render(const Camera& camera) {}

bool EnemyManager::checkPlayerCollision(const Player& player) { return false; }

bool EnemyManager::checkProjectileCollision(float px, float py, float pw, float ph) { return false; }

void EnemyManager::spawnEnemy(float y) {}
