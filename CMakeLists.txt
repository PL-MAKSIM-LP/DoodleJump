cmake_minimum_required(VERSION 3.10)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 17)

include_directories(framework/inc)

add_executable(DoodleJump
    game/game.cpp
    game/Camera.cpp
    game/Player.cpp
    game/Platform.cpp
    game/PlatformManager.cpp
    game/Enemy.cpp
    game/EnemyManager.cpp
    game/Projectile.cpp
    game/ProjectileManager.cpp
    game/ScoreDisplay.cpp
    game/Commands/InputHandler.cpp
)

target_link_libraries(DoodleJump
    ${CMAKE_SOURCE_DIR}/framework/linux/libFrameworkRelease.so
    SDL2
    SDL2_image
)

add_custom_command(TARGET DoodleJump POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/framework/linux/libFrameworkRelease.so.0.0.1
        $<TARGET_FILE_DIR:DoodleJump>/libFrameworkRelease.so.0.0.1
)
