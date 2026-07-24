module player;

#ifdef __INTELLISENSE__
#include "player.cc"
#endif  // __INTELLISENSE__

Player::Player(Position position, Stats baseStats, RaceType raceType)
    : BaseCharacter{position, baseStats, raceType, CharacterMoveStrategyType::PlayerInput} {}

Observer<PlayerActionEvent>* Player::inputObserver() {
  return &static_cast<PlayerInputMoveStrategy&>(movementStrategy());
}

int Player::getGold() const { return gold; }
void Player::addGold(int amount) { gold += amount; }
