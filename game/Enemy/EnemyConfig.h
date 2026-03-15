// EnemyConfig.h — centralized enemy-related constants
#pragma once

namespace EnemyConfig {
// Spawning
constexpr float SPAWN_INTERVAL = 400.0f;  // world-space distance between spawns
constexpr float SPAWN_START_OFFSET =
    2000.0f;  // start spawning when playerY <= screenHeight - OFFSET
constexpr float SPAWN_AHEAD_EXTRA =
    200.0f;  // place spawned enemies this far above player
constexpr int INITIAL_BATCH =
    3;  // initial number of enemies to spawn when enabling

// Placement and removal
constexpr float REMOVE_SCREEN_PADDING =
    200.0f;                   // how far past screen before removing enemy
constexpr int X_MARGIN = 50;  // margin used when sampling random X positions
constexpr float PLATFORM_TOLERANCE =
    40.0f;  // tolerance when matching platform Y for grounded spawn

// Visual sizes
constexpr int GROUND_W = 50;
constexpr int GROUND_H = 40;
constexpr int FLY_W = 40;
constexpr int FLY_H = 30;
}  // namespace EnemyConfig
