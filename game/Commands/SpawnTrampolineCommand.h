#pragma once
#include "../Bonuses/BonusManager.h"
#include "../GameData/GameData.h"
#include "ICommand.h"
#include "core/Constants.h"

class SpawnTrampolineCommand : public ICommand {
 public:
  explicit SpawnTrampolineCommand(BonusManager* manager)
      : bonusManager(manager) {}

  virtual void execute(Player&) override {
    if (bonusManager && !bonusManager->spawnedTrampoline &&
        GameData::getCoins() >= BonusConfig::TRAMPOLINE_SPAWN_COST) {
      GameData::addCoins(-BonusConfig::TRAMPOLINE_SPAWN_COST);
      bonusManager->spawnedTrampoline = true;
    }
  }

  virtual void undo(Player&) override {}

 private:
  BonusManager* bonusManager;
};
