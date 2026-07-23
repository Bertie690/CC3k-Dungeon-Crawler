module character;

#ifdef __INTELLISENSE__
#include <memory>
#include <variant>
#include <vector>

#include "../enums/tile-type.cc"
#include "../floor/cell.cc"
#include "character-movement.cc"
#else
import <memory>;
import <variant>;
import <vector>;
import action;
import cell;
import floor;
import observer;
import gameevents;
import direction;
import room;
import tiletype;
import :movement;
import :movestrategytype;
#endif  // __INTELLISENSE__

void PlayerInputMoveStrategy::onNotify(const PlayerActionEvent& event) {
  if (const Move* move = std::get_if<Move>(&event.action)) {
    nextAction = *move;
  } else if (const Attack* attack = std::get_if<Attack>(&event.action)) {
    nextAction = *attack;
  } else if (const UsePotion* potion = std::get_if<UsePotion>(&event.action)) {
    nextAction = *potion;
  } else {
    nextAction = Pass{};
  }
}

Action PlayerInputMoveStrategy::getNextMove(Floor&, Character&) {
  Action action = nextAction;
  nextAction = Pass{};
  return action;
}

Action RandomMoveStrategy::getNextMove(Floor& floor, Character& character) {
  const Room& room = floor.getRoomAt(character.position());

  std::vector<Direction> availableAdjacentCells;
  availableAdjacentCells.reserve(8);

  for (const Direction direction : room.getAdjacentCells(character.position())) {
    const Cell& destination = floor.getCell(character.position() + direction);

    // unwalkable cells (walls, doors, etc.) can be discarded entirely
    if (!destination.isWalkable()) {
      continue;
    }

    // Filter out occupied cells if we can't attack them
    if (destination.isOccupied()) {
      for (const auto& entity : destination.getEntities()) {
        const Character* target = dynamic_cast<const Character*>(entity.get());
        if (target && character.canAttack(target->raceType())) {
          // found someone to smack
          return Attack{direction};
        }
      }
      continue;
    }

    if (!RandomMoveStrategy::frozen && destination.tileType == TileType::Floor) {
      availableAdjacentCells.push_back(direction);
    }
  }
  if (availableAdjacentCells.empty()) {
    return Pass{};
  }

  int randomIndex = floor.rng.intRange(static_cast<int>(availableAdjacentCells.size()));
  return Move{availableAdjacentCells[randomIndex]};
};

DragonMoveStrategy::DragonMoveStrategy(const Position& goldPos) : goldPos(goldPos) {}

Action DragonMoveStrategy::getNextMove(Floor& floor, Character& character) {
  const Room& room = floor.getRoomAt(character.position());

  for (const Direction direction : room.getAdjacentCells(character.position())) {
    const Cell& destination = floor.getCell(character.position() + direction);
    if (!destination.isOccupied()) {
      continue;
    }
    for (const auto& entity : destination.getEntities()) {
      const Character* target = dynamic_cast<const Character*>(entity.get());
      if (target && character.canAttack(target->raceType())) {
        // found someone to smack
        return Attack{direction};
      }
    }
  }

  // check next to gold hoard
  // technically duplicates search range slightly, but it's fiiine
  for (const Direction direction : room.getAdjacentCells(goldPos)) {
    const Cell& destination = floor.getCell(goldPos + direction);
    if (!destination.isOccupied()) {
      continue;
    }
    for (const auto& entity : destination.getEntities()) {
      const Character* target = dynamic_cast<const Character*>(entity.get());
      if (target && character.canAttack(target->raceType())) {
        // found someone to smack
        return Attack{direction};
      }
    }
  }

  // nothing nearby to attack, so just pass the turn
  return Pass{};
};
