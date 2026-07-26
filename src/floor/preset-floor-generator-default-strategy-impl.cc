module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "../enums/direction.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "preset-floor-generator.cc"
#include "room.cc"
#else
import <string>;
import <vector>;
import <stdexcept>;
import <optional>;
import <set>;
import direction;
import dragonhoardfactory;
import chamber;
import position;
import floor;
import room;
#endif  // __INTELLISENSE__

std::optional<Position> findAdjacentHoard(const Floor& floor,
                                          const std::set<Position>& hoardPositions,
                                          const Position& dragonPosition) {
  const Room& chamber = floor.getRoomAt(dragonPosition);
  for (const Direction direction : chamber.getAdjacentCells(dragonPosition)) {
    Position candidate = dragonPosition + direction;
    if (hoardPositions.contains(candidate)) {
      return candidate;
    }
  }
  return std::nullopt;
}

void DefaultHoardPlacementStrategy::placeDragonHoards(
    Floor& floor, const std::vector<Position>& hoardPositions,
    const std::vector<Position>& dragonPositions) {
  std::set<Position> unclaimedDragonHoards{hoardPositions.begin(), hoardPositions.end()};

  for (const Position& dragonPosition : dragonPositions) {
    std::optional<Position> hoardPosition =
        findAdjacentHoard(floor, unclaimedDragonHoards, dragonPosition);
    if (!hoardPosition.has_value()) {
      throw std::invalid_argument{"No adjacent Dragon Hoard found for Dragon at " +
                                  std::string(dragonPosition)};
    }
    unclaimedDragonHoards.erase(*hoardPosition);
    dragonHoardFactory.process(floor.getRoomAt(*hoardPosition), *hoardPosition, dragonPosition);
  }
}
