module character:movement;

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "character-movement.cc"
#else
import <vector>;
import <memory>;
#endif  // __INTELLISENSE__

Action PlayerInputMoveStrategy::getNextMove(Floor& floor, Character& character) {
  // TODO: Implement player input move strategy
  return Pass{};
};

Action RandomMoveStrategy::getNextMove(Floor& floor, Character& character) {
  const Room& room = floor.getRoomAt(character.position);

  std::vector<Direction> availableAdjacentCells;
  availableAdjacentCells.reserve(8);

  for (const Direction direction : room.getAdjacentCells(character.position)) {
    const Cell& destination = floor.getCell(character.position + direction);

    // unwalkable cells (walls, doors, etc.) can be discarded entirely
    if (!destination.isWalkable()) {
      continue;
    }

    // Filter out occupied cells if we can't attack them
    if (destination.isOccupied()) {
      for (const auto& entity : destination.getEntities()) {
        const Character* target = dynamic_cast<const Character*>(entity.get());
        if (target && character.canAttack(target->raceType)) {
          // found someone to smack
          return Attack{direction};
        }
      }
      continue;
    }

    // Filter out doors
    if (destination.tileType == TileType::Floor) {
      availableAdjacentCells.push_back(direction);
    }
  }
  if (availableAdjacentCells.empty()) {
    return Pass{};
  }

  int randomIndex = floor.rng.intRange(static_cast<int>(availableAdjacentCells.size() - 1));
  return Move{availableAdjacentCells[randomIndex]};
};

Action StaticMoveStrategy::getNextMove(Floor& floor, Character& character) {
  const Room& room = floor.getRoomAt(character.position);

  for (const Direction direction : room.getAdjacentCells(character.position)) {
    const Cell& destination = floor.getCell(character.position + direction);
    if (!destination.isOccupied()) {
      continue;
    }
    for (const auto& entity : destination.getEntities()) {
      const Character* target = dynamic_cast<const Character*>(entity.get());
      if (target && character.canAttack(target->raceType)) {
        // found someone to smack
        return Attack{direction};
      }
    }
  }

  // nothing nearby to attack, so just pass the turn
  return Pass{};
};
