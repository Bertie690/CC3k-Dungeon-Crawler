export module factorybase;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>

#include "../floor/position.cc"
#include "../floor/room.cc"
#include "../utils/rng.cc"
#else
import <vector>;
import position;
import room;
import rng;
#endif  // __INTELLISENSE__

// FactoryBase is the base class for all entity factories.
export class FactoryBase {
 protected:
  RNG& rng;

 public:
  FactoryBase(RNG& rng) : rng{rng} {}
  virtual ~FactoryBase() = default;

  // Process the given Room. The provided list of availablePositions is used to improve caching purposes.
  virtual void process(Room& room, std::vector<Position>& availablePositions) = 0;
};
