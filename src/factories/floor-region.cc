export module floorregion;

#pragma once

#ifdef __INTELLISENSE__
#include <set>
#include <stdexcept>

#include "../floor/position.cc"
#include "../floor/room.cc"
#include "../utils/rng.cc"
#else
import <set>;
import <stdexcept>;
import position;
import rng;
import room;
#endif  // __INTELLISENSE__

// A FloorRegion represents a contiguous, partially-built Room inside a given Floor.
export struct FloorRegion {
  // A non-owning reference to the Room that this FloorRegion represents.
  Room& room;
  // A set of all yet-unfilled positions that are part of this FloorRegion.
  // Used to allow easy caching of available positions for entity placement,
  // and **must be updated whenever a position is filled**.
  std::set<Position> availablePositions;
};

// Select and remove a uniformly chosen random Position from this region.
export Position selectRandomSpawn(FloorRegion& region, RNG& rng) {
  if (region.availablePositions.empty()) {
    throw std::out_of_range{"No valid Cells available for spawning"};
  }
  Position selected = rng.pick(region.availablePositions);
  region.availablePositions.erase(selected);
  return selected;
}
