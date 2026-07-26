export module standardfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "../entities/entity.cc"
#include "../floor/cell.cc"
#include "../floor/position.cc"
#include "../floor/room.cc"
#include "factory-base.cc"
#else
import <memory>;
import <stdexcept>;
import <type_traits>;
import <vector>;
import cell;
import entity;
import factorybase;
import position;
import room;
#endif  // __INTELLISENSE__

// A StandardFactory is a class used for placing Entities on the Floor.
export template<typename T>
  // Only allow T if it inherits from or is of type Entity.
  requires std::is_base_of_v<Entity, T>
class StandardFactory : public FactoryBase {
  // Create an Entity at the given position.
  virtual std::shared_ptr<T> create(const Position& position) = 0;

 public:
  using FactoryBase::FactoryBase;
  // Create an Entity within the Room using the reserved list of avalablePositions.
  virtual void process(Room& room, std::vector<Position>& availablePositions) override;
};

// Implementation
template <typename T>
  requires std::is_base_of_v<Entity, T>
void StandardFactory<T>::process(Room& room, std::vector<Position>& availablePositions) {
  if (availablePositions.empty()) {
    throw std::out_of_range{"No valid Cells available for spawning"};
  }
  std::size_t selectedIndex = rng.intRange(availablePositions.size());
  Position selectedPosition = availablePositions[selectedIndex];
  availablePositions[selectedIndex] = availablePositions.back();
  availablePositions.pop_back();

  room[selectedPosition].add(create(selectedPosition));
}
