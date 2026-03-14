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
    int screenWidth;
    int screenHeight;

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
    virtual void PreInit(int& width, int& height, bool& fullscreen) override
    {
       screenWidth = width; 
       screenHeight = height;
       fullscreen = false;
    }

    virtual bool Init()
    {
        background = createSprite("../data/jungle-bck@2x.png");
        setSpriteSize(background, screenHeight, screenWidth);
        return true;
    }

    virtual void Close()
    {
        destroySprite(background);
    }

    virtual bool Tick()
    {
        return false;
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative)
    {
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased)
    {
    }

    virtual void onKeyPressed(FRKey k)
    {
    }

    virtual void onKeyReleased(FRKey k)
    {
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
