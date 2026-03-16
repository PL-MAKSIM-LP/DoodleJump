cmake_minimum_required(VERSION 3.10)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 17)

include(CTest)

include_directories(framework/inc)
include_directories(game)

file(GLOB_RECURSE GAME_SOURCES "game/**/*.cpp" "game/*.cpp")

add_executable(DoodleJump ${GAME_SOURCES})

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

if(BUILD_TESTING)
    add_executable(DoodleJumpTests
        tests/minimal_tests.cpp
        tests/framework_stubs.cpp
        game/Camera/Camera.cpp
        game/Player/Player.cpp
        game/Commands/InputHandler.cpp
        game/projectile/Projectile.cpp
        game/Enemy/EnemyFactory.cpp
        game/Enemy/GroundedEnemy.cpp
        game/Enemy/FlyingEnemy.cpp
        game/Enemy/EnemyManager.cpp
    )

    add_test(NAME DoodleJumpTests COMMAND DoodleJumpTests)
endif()
