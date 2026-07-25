module character;

#ifdef __INTELLISENSE__
#include <memory>
#include <variant>
#include <vector>

#include "../enums/tile-type.cc"
#include "../floor/cell.cc"
#include "character.cc"
#include <utility>
#else
import <memory>;
import <variant>;
import <vector>;
import <utility>;
import action;
import cell;
import floor;
import observer;
import gameevents;
import direction;
import room;
import tiletype;
#endif  // __INTELLISENSE__

void PlayerInputMoveStrategy::onNotify(const PlayerActionEvent& event) {
  nextAction = event.action;
}

Action PlayerInputMoveStrategy::getNextMove(Floor&, Character& character) {
  return std::exchange(nextAction, Pass{});
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

  return Move{floor.rng.pick(availableAdjacentCells)};
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

  const Position relativeGoldPos = goldPos - character.position();

  for (const Direction direction : room.getAdjacentCells(goldPos)) {
    const Cell& destination = floor.getCell(goldPos + direction);
    if (!destination.isOccupied()) {
      continue;
    }
    for (const auto& entity : destination.getEntities()) {
      const Character* target = dynamic_cast<const Character*>(entity.get());
      if (target && character.canAttack(target->raceType())) {
        // found someone to smack
        return Attack{relativeGoldPos + direction};
      }
    }
  }

  // nothing nearby to attack, so just pass the turn
  return Pass{};
};
