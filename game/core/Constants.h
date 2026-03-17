// Centralized game constants to avoid magic numbers in source files
#pragma once

namespace GameConfig {
constexpr int DEFAULT_WIDTH = 390;
constexpr int DEFAULT_HEIGHT = 844;
constexpr int MAX_WIDTH = 1920;
constexpr int MAX_HEIGHT = 1080;
}  // namespace GameConfig

namespace PlayerConfig {
constexpr int JumpHeight = 600;

}  // namespace PlayerConfig
namespace CameraConfig {
constexpr float DESIRED_SCREEN_Y_FRAC = 0.40f;
}

// ========== PLATFORM CONFIG ==========
namespace PlatformConfig {
constexpr int WIDTH = 70;
constexpr int HEIGHT = 18;

// Sprite paths
constexpr const char* SPRITE_NORMAL_PATH = "../sprites/normal.png";
constexpr const char* SPRITE_MOVING_H_PATH = "../sprites/moveHorizontal.png";
constexpr const char* SPRITE_MOVING_V_PATH = "../sprites/moveVertical.png";
constexpr const char* SPRITE_BREAKABLE_PATH = "../sprites/break1.png";
}  // namespace PlatformConfig

namespace PlatformManagerConfig {
constexpr float START_SPAWN_HEIGHT = 20.0f;
constexpr int MAX_ACTIVE_PLATFORMS = 30;
constexpr float BASE_OFFSET = 500.0f;
constexpr float EASY_OFFSET = 500.0f;
constexpr float MEDIUM_OFFSET = 2000.0f;
constexpr float HARD_OFFSET = 4000.0f;
constexpr float LOW_GAP = 50.0f;
constexpr float MID_GAP = 80.0f;
constexpr float HIGH_GAP = 100.0f;
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
constexpr float MOVE_SPEED = 20.0f;
constexpr float SWING = 50.0f;
}  // namespace MovingPlatformConfig

namespace BreakablePlatformConfig {
constexpr int MAX_BREAK_SPRITES = 4;  // Number of break animation frames
constexpr int MAX_BREAK_STAGES = 3;  // Last stage before falling
constexpr float BREAK_STEP_DURATION = 0.12f;
constexpr float FALL_SPEED = 200.0f;
constexpr float OFFSCREEN_PADDING = 1000.0f;

// Breakable platform sprite paths
constexpr const char* SPRITE_BREAK_PATHS[MAX_BREAK_SPRITES] = {
    "../sprites/break1.png",
    "../sprites/break2.png",
    "../sprites/break3.png",
    "../sprites/break4.png",
};
}  // namespace BreakablePlatformConfig

// ========== ENEMY CONFIG ==========
namespace EnemyConfig {
// Sprite paths
constexpr const char* SPRITE_GROUNDED_PATH = "../sprites/enemyGrounded.png";
constexpr const char* SPRITE_FLYING_FALLBACK = "../sprites/enemyFlying.png";
constexpr const char* SPRITE_FLYING_BASE_PATH = "../sprites/flying";  // for {1,2,3}.png

// Animation
constexpr int FLY_FRAMES_COUNT = 3;
constexpr int GROUNDED_WALK_FRAMES_COUNT = 3;

// Spawning
constexpr float SPAWN_INTERVAL = 150.0f;  // world-space distance between spawns
constexpr float SPAWN_START_OFFSET =
    2000.0f;  // start spawning when playerY <= screenHeight - OFFSET
constexpr float SPAWN_AHEAD_EXTRA =
    200.0f;  // place spawned enemies this far above player
constexpr int INITIAL_BATCH =
    5;  // initial number of enemies to spawn when enabling
constexpr int DESIRED_ENEMIES = 5;        // target number of active enemies
constexpr int SPAWN_X_ATTEMPTS = 8;       // attempts to find valid X position
constexpr float MIN_HORIZONTAL_SPACING = 500.0f;  // minimum distance between spawned enemies

// Placement and removal
constexpr float REMOVE_SCREEN_PADDING =
    200.0f;                   // how far past screen before removing enemy
constexpr int X_MARGIN = 50;  // margin used when sampling random X positions
constexpr float PLATFORM_TOLERANCE =
    40.0f;  // tolerance when matching platform Y for grounded spawn

// Visual sizes
constexpr int GROUND_W = 130;
constexpr int GROUND_H = 70;
constexpr int FLY_W = 60;
constexpr int FLY_H = 60;
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
constexpr int TRAMPOLINE_LIFE_TIME = 20000;  // milliseconds

// Misc
constexpr float COIN_PLATFORM_OFFSET = 18.0f;
}  // namespace BonusConfig

// ========== COINS & CURRENCY CONFIG ==========
namespace CoinsConfig {
constexpr int COIN_VALUE = 10;
constexpr int START_VALUE = 50;
constexpr int TRAMPOLINE_VALUE = 20;
}  // namespace CoinsConfig