module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <memory>
#include <variant>

#include "character.cc"
#include "../events/floor-events.cc"
#else
import <cmath>;
import <memory>;
import <variant>;
import floor;
import cell;
import action;
import floorevents;
#endif  // __INTELLISENSE__

unsigned int Character::atk() const { return this->getStats().atk; }
unsigned int Character::def() const { return this->getStats().def; }
unsigned int Character::maxHp() const { return this->getStats().maxHp; }
Stats Character::stats() const { return this->getStats(); }
bool Character::dead() const { return this->isDead(); }

// defaults for non-player characters
Observer<PlayerActionEvent>* Character::inputObserver() { return nullptr; }
int Character::getGold() const { return 0; }
void Character::addGold(int) {}

void Character::attack(Character& defender, Floor& floor) {
  const unsigned int attacks = this->getAttacksPerTurn();
  for (unsigned int i = 0; i < attacks; i++) {
    const double accuracy = this->getAccuracy(defender);
    const double evasion = defender.getEvasion(*this);

    if (floor.rng.randDouble() > (accuracy / evasion)) {
      floor.reportCharacterAction(
          CharacterActionEvent{*this, defender, CharacterActionEvent::Result::Miss});
      continue;
    }

    const double damageMultiplier = this->getAttackDamageMultiplier(defender);
    // ceiling((100/(100 + Def (Defender))) * Atk(Attacker))
    const unsigned int damage =
        std::ceil(100.0 / (100.0 + defender.def()) * this->atk() * damageMultiplier);
    const unsigned int hpBefore = defender.currentHp();
    defender.damage(damage, true, this);
    const unsigned int damageDealt = hpBefore - defender.currentHp();
    this->onHit(defender, damage);
    defender.onBeingAttacked(*this, damage);
    if (defender.dead() && !isPlayer(defender.raceType())) {

      int baseAddedGold = 0;
      RaceType deadRaceType = defender.raceType();
      if (deadRaceType != RaceType::Human && deadRaceType != RaceType::Merchant && deadRaceType != RaceType::Dragon) {
        baseAddedGold += floor.rng.intRange(2) + 1;
      }
      // add gold to the Player
      addGold(baseAddedGold + getExtraGoldDrop());
      floor.reportCharacterAction(CharacterActionEvent{
          *this, defender, CharacterActionEvent::Result::Hit, damageDealt, true});
      floor.remove(defender);
      return;
    }

    floor.reportCharacterAction(CharacterActionEvent{
        *this, defender, CharacterActionEvent::Result::Hit, damageDealt, defender.dead()});
  }
}

void Character::act(Floor& floor) {
  Action nextMove = this->getNextMove(floor);

  if (Pass* pass = std::get_if<Pass>(&nextMove)) {
  this->onTurnEnd();
    return;
  }

  if (Move* move = std::get_if<Move>(&nextMove)) {
    floor.move(*this, this->position() + move->dir);
    this->onTurnEnd();
    return;
  }

  if (UsePotion* potion = std::get_if<UsePotion>(&nextMove)) {
    // TODO: Implement potion usage
  this->onTurnEnd();
    return;
  }

  Attack attack = std::get<Attack>(nextMove);
  const Cell& c = floor.getCell(this->position() + attack.dir);

  Character* character = nullptr;
  for (const auto& entity : c.getEntities()) {
    character = dynamic_cast<Character*>(entity.get());
    if (character) {
      break;
    }
  }

  if (!character) {
    throw std::invalid_argument("No character to attack in the given direction.");
  }

  this->attack(*character, floor);
  this->onTurnEnd();
}

void Character::drain(Character& defender, unsigned int hp) {
  defender.damage(hp);
  if (const double mult = defender.getDrainMulti(*this); mult > 0.0) {
    this->heal(hp * mult);
  } else {
    this->damage(hp * static_cast<int>(-mult));
  }
}
