#pragma once
#include "../Bonuses/BonusManager.h"
#include "../Constants.h"
#include "../GameData/GameData.h"
#include "ICommand.h"

class SpawnTrampolineCommand : public ICommand {
 public:
  explicit SpawnTrampolineCommand(BonusManager* manager)
      : bonusManager(manager) {}

  virtual void execute(Player&) override {
    if (bonusManager && !bonusManager->spawnedTrampoline &&
        GameData::getCoins() >= CoinsConfig::TRAMPOLINE_VALUE) {
      GameData::addCoins(-CoinsConfig::TRAMPOLINE_VALUE);
      bonusManager->spawnedTrampoline = true;
    }
  }

  virtual void undo(Player&) override {}

 private:
  BonusManager* bonusManager;
};
