
#pragma once

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

constexpr int INITIAL_SPAWN_X = 100;

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
constexpr const char* SPRITE_FLYING_BASE_PATH =
    "../sprites/enemy/flying";  // for {1,2,3,4}.png

constexpr int FLY_FRAMES_COUNT = 4;

constexpr float SPAWN_START_OFFSET = 2000.0f;
constexpr float SPAWN_OFFSCREEN_PADDING = 100.0f;
constexpr int MAX_ENEMIES = 3;

constexpr float REMOVE_SCREEN_PADDING = 200.0f;
constexpr int X_MARGIN = 50;

constexpr int FLY_W = 60;
constexpr int FLY_H = 60;
}  // namespace EnemyConfig

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

constexpr int STARTING_COINS = 50;
constexpr int COIN_VALUE = 10;
constexpr int COIN_W = 24;
constexpr int COIN_H = 24;
constexpr int MAX_TOTAL_COINS = 10;
constexpr int MAX_SCREEN_COINS = 3;
constexpr float COIN_SPAWN_CHANCE = 0.30f;
constexpr const char* COIN_SPRITE_PATH = "../sprites/bonuses/coin.png";

constexpr int TRAMPOLINE_SPAWN_COST = 20;
constexpr int TRAMPOLINE_W = 70;
constexpr int TRAMPOLINE_H = 20;
constexpr float TRAMPOLINE_BOOST_MULT = 1.7f;
constexpr int TRAMPOLINE_LIFE_TIME = 30000;  // milliseconds
constexpr const char* TRAMPOLINE_SPRITE_PATH =
    "../sprites/bonuses/trampoline.png";

constexpr float REMOVE_SCREEN_PADDING = 200.0f;
}  // namespace BonusConfig

namespace ScoreConfig {
constexpr const char* DIGIT_SPRITE_PATHS[10] = {
    "../sprites/score/0.png", "../sprites/score/1.png",
    "../sprites/score/2.png", "../sprites/score/3.png",
    "../sprites/score/4.png", "../sprites/score/5.png",
    "../sprites/score/6.png", "../sprites/score/7.png",
    "../sprites/score/8.png", "../sprites/score/9.png",
};

constexpr const char* BG_SPRITE_PATH = "../sprites/score/scoreBG.png";
}  // namespace ScoreConfig

namespace DebugConfig {
constexpr const char* DEFAULT_SPRITE_PATH = "../sprites/fallback.png";
constexpr const char* GAME_OVER_SPRITE_PATH = "../sprites/gameover.png";
}  // namespace DebugConfig

namespace BGConfig {
constexpr const char* BG_SPRITE_PATH = "../sprites/jungle-bck@2x.png";
}
