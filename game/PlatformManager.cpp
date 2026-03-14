#include "PlatformManager.h"
#include <cstdlib>
#include <algorithm>

PlatformManager::PlatformManager() {}

PlatformManager::~PlatformManager() {}

void PlatformManager::init(int w, int h) {}

void PlatformManager::update(float dt, int screenWidth, const Camera& camera) {}

void PlatformManager::render(const Camera& camera) {}

bool PlatformManager::checkCollision(Player& player) { return false; }

void PlatformManager::spawnPlatform(float y) {}

void PlatformManager::fillInitial() {}
