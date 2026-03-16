// Centralized game constants to avoid magic numbers in source files
#pragma once

namespace GameConfig {
constexpr int DEFAULT_WIDTH = 390;
constexpr int DEFAULT_HEIGHT = 844;
constexpr int MAX_WIDTH = 1920;
constexpr int MAX_HEIGHT = 1080;
}  // namespace GameConfig

namespace CameraConfig {
constexpr float DESIRED_SCREEN_Y_FRAC = 0.40f;
}

// ========== PLATFORM CONFIG ==========
namespace PlatformConfig {
constexpr int WIDTH = 70;
constexpr int HEIGHT = 18;
}  // namespace PlatformConfig

namespace PlatformManagerConfig {
constexpr float SPAWN_GAP_Y = 100.0f;  // base gap fallback
constexpr int MAX_ACTIVE = 80;
constexpr float EASY_OFFSET = 500.0f;
constexpr float MEDIUM_OFFSET = 2000.0f;
constexpr float HARD_OFFSET = 4000.0f;
constexpr float LOW_GAP = 80.0f;
constexpr float MID_GAP = 110.0f;
constexpr float HIGH_GAP = 150.0f;
constexpr float RAND_R_MIN = 0.0f;
constexpr float RAND_R_MAX = 1.0f;
// initial spawn X
constexpr int INITIAL_SPAWN_X = 100;
// probabilities for platform selection (threshold comparisons)
constexpr float PROB_MEDIUM_NORMAL = 0.80f;
constexpr float PROB_VERYHIGH_NORMAL = 0.60f;
constexpr float PROB_VERYHIGH_MOVINGH = 0.90f;
constexpr float PROB_HIGHEST_NORMAL = 0.40f;
constexpr float PROB_HIGHEST_MOVINGH = 0.75f;
constexpr float PROB_HIGHEST_MOVINGV = 0.90f;
}  // namespace PlatformManagerConfig

namespace MovingPlatformConfig {
constexpr float MOVE_SPEED = 10.0f;
constexpr float SWING = 50.0f;
}  // namespace MovingPlatformConfig

namespace BreakablePlatformConfig {
constexpr int MAX_BREAK_STAGES = 3;
constexpr float BREAK_STEP_DURATION = 0.12f;
constexpr float FALL_SPEED = 200.0f;
constexpr float OFFSCREEN_PADDING = 1000.0f;
}  // namespace BreakablePlatformConfig

// ========== ENEMY CONFIG ==========
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

// ========== PROJECTILE CONFIG ==========
namespace ProjectileConfig {
constexpr int W = 10;
constexpr int H = 20;
constexpr float DEFAULT_VELY = -600.0f;
constexpr float SHOOT_SPEED = 700.0f;
constexpr float OFFSCREEN_PADDING = 200.0f;
}  // namespace ProjectileConfig

// ========== BONUS CONFIG ==========
namespace BonusConfig {
// Spawn
constexpr float SPAWN_INTERVAL = 500.0f;
constexpr float SPAWN_START_OFFSET = 1200.0f;
constexpr float SPAWN_AHEAD_EXTRA = 200.0f;
constexpr int INITIAL_BATCH = 2;

// Removal
constexpr float REMOVE_SCREEN_PADDING = 200.0f;

// Coin config
constexpr int COIN_VALUE = 10;
constexpr int COIN_W = 24;
constexpr int COIN_H = 24;

// Trampoline config
constexpr int TRAMPOLINE_VALUE = 15;
constexpr int TRAMPOLINE_W = 70;
constexpr int TRAMPOLINE_H = 20;
constexpr float TRAMPOLINE_PROBABILITY = 0.20f;
constexpr float TRAMPOLINE_BOOST_MULT = 1.7f;
constexpr int TRAMPOLINE_LIFE_TIME = 5000;  // milliseconds

// Misc
constexpr float COIN_PLATFORM_OFFSET = 18.0f;
}  // namespace BonusConfig

// ========== COINS & CURRENCY CONFIG ==========
namespace CoinsConfig {
constexpr int COIN_VALUE = 10;
constexpr int START_VALUE = 50;
constexpr int TRAMPOLINE_VALUE = 15;
}  // namespace CoinsConfig
