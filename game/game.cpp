#include "../framework/Framework.h"
#include "GameState.h"
#include "Camera.h"
#include "Player.h"
#include "PlatformManager.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "ScoreDisplay.h"

class MyFramework : public Framework
{
    int screenWidth  = 500;
    int screenHeight = 500;

    GameState state = GameState::MENU;

    Sprite*           background       = nullptr;
    Camera            camera;
    Player            player;
    PlatformManager   platformManager;
    EnemyManager      enemyManager;
    ProjectileManager projectileManager;
    ScoreDisplay      scoreDisplay;

    unsigned int lastTick = 0;

public:
    virtual void PreInit(int& width, int& height, bool& fullscreen)
    {
        width      = screenWidth;
        height     = screenHeight;
        fullscreen = false;
    }

    virtual bool Init()
    {
        background = createSprite("../data/jungle-bck@2x.png");
        setSpriteSize(background, screenWidth, screenHeight);

        player.init(screenWidth, screenHeight);
        platformManager.init(screenWidth, screenHeight);
        enemyManager.init(screenWidth, screenHeight);
        projectileManager.init();

        state     = GameState::PLAYING;
        lastTick  = getTickCount();
        return true;
    }

    virtual void Close()
    {
        destroySprite(background);
    }

    virtual bool Tick()
    {
        unsigned int now = getTickCount();
        float dt = (now - lastTick) / 1000.0f;
        lastTick = now;
        // Cap dt to avoid large jumps after pauses
        if (dt > 0.05f) dt = 0.05f;

        if (state == GameState::PLAYING)
        {
            player.update(dt, screenWidth);
            platformManager.update(dt, screenWidth, camera);
            enemyManager.update(dt, screenWidth, camera);
            projectileManager.update(dt, camera);

            platformManager.checkCollision(player);

            // Check projectile vs enemy collisions
            for (auto& proj : projectileManager.getProjectiles()) {
                if (!proj.active) continue;
                if (enemyManager.checkProjectileCollision(proj.left(), proj.top(), proj.w, proj.h)) {
                    proj.active = false;
                    scoreDisplay.setScore(scoreDisplay.getScore() + 100);
                }
            }

            // Player hit by enemy -> game over
            if (enemyManager.checkPlayerCollision(player))
                state = GameState::GAME_OVER;

            // Player fell below screen -> game over
            if (camera.toScreenY(player.y) > screenHeight)
                state = GameState::GAME_OVER;

            camera.update(player.y, screenHeight);

            // Score increases with height
            int heightScore = (int)(-camera.offsetY / 10.0f);
            if (heightScore > scoreDisplay.getScore())
                scoreDisplay.setScore(heightScore);
        }

        // Render
        drawSprite(background, 0, 0);
        platformManager.render(camera);
        enemyManager.render(camera);
        projectileManager.render(camera);
        player.render(camera);
        scoreDisplay.render();

        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {}

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
    {
        // Left click shoots
        if (button == FRMouseButton::LEFT && !isReleased && state == GameState::PLAYING)
            projectileManager.shoot(player.x + player.w / 2.0f, player.y);
    }

    virtual void onKeyPressed(FRKey k)
    {
        if (state == GameState::PLAYING)
            player.onKeyPressed(k);

        // Restart on any key after game over
        if (state == GameState::GAME_OVER) {
            player.init(screenWidth, screenHeight);
            platformManager.init(screenWidth, screenHeight);
            enemyManager.init(screenWidth, screenHeight);
            camera = Camera();
            scoreDisplay.setScore(0);
            state = GameState::PLAYING;
        }
    }

    virtual void onKeyReleased(FRKey k)
    {
        if (state == GameState::PLAYING)
            player.onKeyReleased(k);
    }

    virtual const char* GetTitle() override
    {
        return "DoodleJump";
    }
};

int main(int argc, char* argv[])
{
    return run(new MyFramework);
}
