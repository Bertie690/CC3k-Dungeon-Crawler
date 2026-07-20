export module racetype;

#pragma once

export enum class RaceType {
  // players
  Shade,
  Drow,
  Vampire,
  Troll,
  Goblin,

  // enemies

  Human,
  Dwarf,
  Elf,
  Orc,
  Merchant,
  Dragon,
  Halfling,
};

// Return whether the given race type corresponds to a player character.
// Used to avoid cyclic dependencies among Character, Enemy and Player pertaining to attack logic.
export bool isPlayer(RaceType race) {
  switch (race) {
    case RaceType::Shade:
    case RaceType::Drow:
    case RaceType::Vampire:
    case RaceType::Troll:
    case RaceType::Goblin:
      return true;
  }
  return false;
}
