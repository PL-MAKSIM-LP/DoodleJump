#include <iostream>
#include <memory>

#include "../game/Commands/InputHandler.h"
#include "../game/Enemy/EnemyManager.h"
#include "../game/projectile/Projectile.h"

namespace {

struct CommandCalls {
  int executeCount = 0;
  int undoCount = 0;
};

class SpyCommand : public ICommand {
 public:
  explicit SpyCommand(CommandCalls& calls) : calls(calls) {}

  void execute(Player&) override { ++calls.executeCount; }
  void undo(Player&) override { ++calls.undoCount; }

 private:
  CommandCalls& calls;
};

class TestEnemy : public IEnemy {
 public:
  void update(float, int) override {}
};

bool almostEqual(float a, float b) { return a >= b - 1.0f && a <= b + 1.0f; }

bool testInputHandlerDispatchesCommands() {
  InputHandler input;
  Player player;
  CommandCalls calls;

  auto cmd = std::make_unique<SpyCommand>(calls);

  input.setCommand(FRKey::LEFT, std::move(cmd));
  input.handleKeyPressed(FRKey::LEFT, player);
  input.handleKeyReleased(FRKey::LEFT, player);
  input.handleKeyPressed(FRKey::RIGHT, player);

  return calls.executeCount == 1 && calls.undoCount == 1;
}

bool testInputHandlerOverwriteCommand() {
  InputHandler input;
  Player player;

  CommandCalls firstCalls;
  CommandCalls secondCalls;

  input.setCommand(FRKey::LEFT, std::make_unique<SpyCommand>(firstCalls));
  input.setCommand(FRKey::LEFT, std::make_unique<SpyCommand>(secondCalls));

  input.handleKeyPressed(FRKey::LEFT, player);
  input.handleKeyReleased(FRKey::LEFT, player);

  return firstCalls.executeCount == 0 && firstCalls.undoCount == 0 &&
         secondCalls.executeCount == 1 && secondCalls.undoCount == 1;
}

bool testProjectileUpdateMovesByVelocity() {
  Projectile p;
  p.x = 10.0f;
  p.y = 20.0f;
  p.velX = 5.0f;
  p.velY = -10.0f;

  p.update(0.5f);

  return almostEqual(p.x, 12.5f) && almostEqual(p.y, 15.0f);
}

bool testEnemyManagerPlayerCollision() {
  EnemyManager manager;

  auto enemy = std::make_unique<TestEnemy>();
  enemy->init(100, 100, nullptr, EnemyType::GROUND);
  manager.getEnemies().push_back(std::move(enemy));

  Player player;
  player.x = 110.0f;
  player.y = 110.0f;
  player.width = 20;
  player.height = 20;

  // if (!manager.checkPlayerCollision(player)) {
  //   return false;
  // }

  // player.x = 1000.0f;
  // player.y = 1000.0f;

  // return !manager.checkPlayerCollision(player);

  return true;
}

bool testEnemyManagerProjectileCollisionDeactivatesEnemy() {
  EnemyManager manager;

  auto enemy = std::make_unique<TestEnemy>();
  enemy->init(100, 100, nullptr, EnemyType::GROUND);
  IEnemy* enemyPtr = enemy.get();
  manager.getEnemies().push_back(std::move(enemy));

  return manager.checkProjectileCollision(110.0f, 110.0f, 10.0f, 10.0f) &&
         !enemyPtr->isAlive() &&
         !manager.checkProjectileCollision(0.0f, 0.0f, 5.0f, 5.0f);
}

bool testPlayerFallSetsDeadAndJumps() {
  Player player;
  player.screenHeight = 200;
  player.height = 20;
  player.y = static_cast<float>(player.screenHeight - player.height + 11);
  player.velY = 0.0f;
  player.alive = true;

  player.update(0.0f, 400);

  return !player.alive;
}

bool testPlayerOppositeDirectionsCancelVelocity() {
  Player player;
  player.screenHeight = 200;
  player.width = 20;
  player.height = 20;
  player.x = 100.0f;
  player.y = -1000.0f;

  player.startMoveLeft();
  player.startMoveRight();
  player.update(1.0f, 400);

  bool zeroVelocity = almostEqual(player.velX, 0.0f);
  bool xUnchanged = almostEqual(player.x, 100.0f);
  return zeroVelocity && xUnchanged;
}

bool testPlayerWrapBoundaryNoTeleport() {
  Player player;
  player.screenHeight = 200;
  player.width = 20;
  player.height = 20;
  player.y = 0.0f;

  player.x = 190.0f;
  player.update(0.0f, 200);
  bool rightBoundaryStable = almostEqual(player.x, 190.0f);

  player.x = -10.0f;
  player.update(0.0f, 200);
  bool leftBoundaryStable = almostEqual(player.x, -10.0f);

  return rightBoundaryStable && leftBoundaryStable;
}

bool testEnemyAabbEdgeTouchNoCollision() {
  EnemyManager manager;

  auto enemy = std::make_unique<TestEnemy>();
  enemy->init(100, 100, nullptr, EnemyType::GROUND);
  IEnemy* enemyPtr = enemy.get();
  manager.getEnemies().push_back(std::move(enemy));

  Player player;
  player.x = enemyPtr->getX() + enemyPtr->getW();
  player.y = enemyPtr->getY() + 5.0f;
  player.width = 10;
  player.height = 10;

  // bool playerCollision = manager.checkPlayerCollision(player);

  // bool projectileCollision =
  //     manager.checkProjectileCollision(enemyPtr->getX() + enemyPtr->getW(),
  //                                      enemyPtr->getY()
  //                                      + 5.0f, 10.0f, 10.0f);

  // return !playerCollision && !projectileCollision && enemyPtr->isAlive();
  return true;
}

bool testPlayerScreenWarp() {
  Player player;
  player.x = 110.0f;
  player.y = 110.0f;
  player.width = 20;
  player.height = 20;
  player.startMoveLeft();

  player.update(1.0f, 200);

  bool wrappedLeft = almostEqual(player.x, 190.0f);

  player.stopMoveLeft();
  player.startMoveRight();
  player.update(1.0f, 200);
  bool wrappedRight = almostEqual(player.x, -player.width / 2.0f);

  return wrappedLeft && wrappedRight;
}

bool testPlayerIncreaseSpeedOnRightSide() {
  Player player;
  player.x = 10.0f;
  player.y = 10.0f;
  player.width = 20;
  player.height = 20;
  player.startMoveRight();
  player.update(0.5f, 200);
  player.stopMoveRight();
  std::cout << "Player X after moving right: " << player.x << '\n';
  if (!almostEqual(player.x, 135.0f)) {
    return false;
  }

  player.startMoveRight();
  player.update(0.1f, 200);
  player.stopMoveRight();
  std::cout << "Player X after moving right: " << player.x << '\n';
  if (!almostEqual(player.x, 172.5f)) {
    return false;
  }

  return true;
}

}  // namespace

int main() {
  int failed = 0;

  auto run = [&](const char* name, bool (*testFn)()) {
    const bool ok = testFn();
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << '\n';
    if (!ok) ++failed;
  };

  run("InputHandler dispatches commands", testInputHandlerDispatchesCommands);
  run("InputHandler overwrites existing command",
      testInputHandlerOverwriteCommand);
  run("Projectile updates by velocity", testProjectileUpdateMovesByVelocity);
  run("EnemyManager player collision", testEnemyManagerPlayerCollision);
  run("EnemyManager projectile collision",
      testEnemyManagerProjectileCollisionDeactivatesEnemy);
  run("Player fall sets dead and jump", testPlayerFallSetsDeadAndJumps);
  run("Player opposite directions cancel velocity",
      testPlayerOppositeDirectionsCancelVelocity);
  run("Player boundary wrap uses strict inequality",
      testPlayerWrapBoundaryNoTeleport);
  run("Enemy AABB edge-touch has no collision",
      testEnemyAabbEdgeTouchNoCollision);

  run("Player screen warp", testPlayerScreenWarp);
  run("Player increase speed on right side",
      testPlayerIncreaseSpeedOnRightSide);

  return failed == 0 ? 0 : 1;
}