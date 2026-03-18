// Centralized game constants to avoid magic numbers in source files
#pragma once

// Game difficulty levels
enum class GameDifficulty { EASY, MEDIUM, HARD };

namespace GameConfig {
constexpr int DEFAULT_WIDTH = 390;
constexpr int DEFAULT_HEIGHT = 844;
constexpr int MAX_WIDTH = 1920;
constexpr int MAX_HEIGHT = 1080;
constexpr GameDifficulty GAME_DIFFICULTY = GameDifficulty::MEDIUM;
}  // namespace GameConfig

namespace PlayerConfig {
constexpr int JumpHeight = 300;
constexpr const char* SPRITE_LEFT_PATH = "../sprites/player/lik-left.png";
constexpr const char* SPRITE_RIGHT_PATH = "../sprites/player/lik-right.png";
}  // namespace PlayerConfig

namespace CameraConfig {
constexpr float DESIRED_SCREEN_Y_FRAC = 0.40f;
}  // namespace CameraConfig

namespace PlatformConfig {
constexpr int WIDTH = 70;
constexpr int HEIGHT = 18;

// Sprite paths
constexpr const char* SPRITE_NORMAL_PATH = "../sprites/platforms/normal.png";
constexpr const char* SPRITE_MOVING_H_PATH =
    "../sprites/platforms/moveHorizontal.png";
constexpr const char* SPRITE_MOVING_V_PATH =
    "../sprites/platforms/moveVertical.png";
constexpr const char* SPRITE_BREAKABLE_PATH = "../sprites/platforms/break1.png";
}  // namespace PlatformConfig

namespace PlatformManagerConfig {
constexpr float START_SPAWN_HEIGHT = 20.0f;
constexpr int MAX_ACTIVE_PLATFORMS = 30;
constexpr float BASE_OFFSET = 500.0f;
constexpr float EASY_OFFSET = 500.0f;
constexpr float MEDIUM_OFFSET = 2000.0f;
constexpr float HARD_OFFSET = 4000.0f;
constexpr float LOW_GAP = 150.0f;
constexpr float MID_GAP = 200.0f;
constexpr float HIGH_GAP = 250.0f;
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
constexpr int MAX_BREAK_SPRITES = 4;
constexpr int MAX_BREAK_STAGES = 3;
constexpr float BREAK_STEP_DURATION = 0.12f;
constexpr float FALL_SPEED = 200.0f;
constexpr float OFFSCREEN_PADDING = 1000.0f;

constexpr const char* SPRITE_BREAK_PATHS[MAX_BREAK_SPRITES] = {
    "../sprites/platforms/break1.png",
    "../sprites/platforms/break2.png",
    "../sprites/platforms/break3.png",
    "../sprites/platforms/break4.png",
};
}  // namespace BreakablePlatformConfig

namespace EnemyConfig {
// Sprite paths
constexpr const char* SPRITE_FLYING_BASE_PATH =
    "../sprites/enemy/flying";  // for {1,2,3,4}.png

// Animation
constexpr int FLY_FRAMES_COUNT = 4;

// Spawning
constexpr float SPAWN_START_OFFSET = 2000.0f;
constexpr float SPAWN_OFFSCREEN_PADDING = 100.0f;

// Placement and removal
constexpr float REMOVE_SCREEN_PADDING = 200.0f;
constexpr int X_MARGIN = 50;

// Visual sizes
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
constexpr unsigned int SHOOT_LIFETIME = 1000;  // milliseconds

constexpr const char* PROJECTILE_SPRITE_PATH = "../sprites/projectile.png";
}  // namespace ProjectileConfig

// ========== BONUS CONFIG ==========
namespace BonusConfig {
// Game start and currency
constexpr int STARTING_COINS = 50;

// Coin config
constexpr int COIN_VALUE = 10;
constexpr int COIN_W = 24;
constexpr int COIN_H = 24;
constexpr int MAX_TOTAL_COINS = 10;
constexpr int MAX_SCREEN_COINS = 3;
constexpr float COIN_SPAWN_CHANCE = 0.30f;
constexpr const char* COIN_SPRITE_PATH = "../sprites/bonuses/coin.png";

// Trampoline config
constexpr int TRAMPOLINE_SPAWN_COST = 20;  // Cost to spawn trampoline
constexpr int TRAMPOLINE_W = 70;
constexpr int TRAMPOLINE_H = 20;
constexpr float TRAMPOLINE_BOOST_MULT = 1.7f;
constexpr int TRAMPOLINE_LIFE_TIME = 30000;  // milliseconds
constexpr const char* TRAMPOLINE_SPRITE_PATH =
    "../sprites/bonuses/trampoline.png";

// Bonus removal
constexpr float REMOVE_SCREEN_PADDING = 200.0f;
}  // namespace BonusConfig

// ========== SCORE DISPLAY CONFIG ==========
namespace ScoreConfig {
constexpr const char* DIGIT_SPRITE_PATHS[10] = {
    "../sprites/score/0.png",  // 0
    "../sprites/score/1.png",  // 1
    "../sprites/score/2.png",  // 2
    "../sprites/score/3.png",  // 3
    "../sprites/score/4.png",  // 4
    "../sprites/score/5.png",  // 5
    "../sprites/score/6.png",  // 6
    "../sprites/score/7.png",  // 7
    "../sprites/score/8.png",  // 8
    "../sprites/score/9.png",  // 9
};
}  // namespace ScoreConfig

namespace DebugConfig {
constexpr const char* DEFAULT_SPRITE_PATH = "../sprites/fallback.png";
}

namespace BGConfig {
constexpr const char* BG_SPRITE_PATH = "../sprites/jungle-bck@2x.png";
}
