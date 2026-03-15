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
constexpr int MAX_BREAK_STAGES = 4;
constexpr float BREAK_STEP_DURATION = 0.12f;
constexpr float FALL_SPEED = 200.0f;
constexpr float OFFSCREEN_PADDING = 1000.0f;
}  // namespace BreakablePlatformConfig

namespace EnemyPlacementConfig {
constexpr float REMOVE_SCREEN_PADDING = 200.0f;
constexpr int X_MARGIN = 50;
constexpr float PLATFORM_TOLERANCE = 40.0f;
}  // namespace EnemyPlacementConfig

namespace ProjectileConfig {
constexpr int W = 10;
constexpr int H = 20;
constexpr float DEFAULT_VELY = -600.0f;
constexpr float SHOOT_SPEED = 700.0f;
constexpr float OFFSCREEN_PADDING = 200.0f;
}  // namespace ProjectileConfig
