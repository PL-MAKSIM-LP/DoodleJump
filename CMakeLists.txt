cmake_minimum_required(VERSION 3.10)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 17)

# Option to enable AddressSanitizer build
option(ENABLE_ASAN "Enable AddressSanitizer (adds -fsanitize=address)" OFF)
if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer: ON")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g -O1")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    else()
        message(WARNING "ENABLE_ASAN set but compiler is not GCC/Clang; skipping flags")
    endif()
endif()

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
