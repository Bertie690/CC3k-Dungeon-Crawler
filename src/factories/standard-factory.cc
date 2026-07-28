export module standardfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <functional>
#include <memory>
#include <set>
#include <stdexcept>
#include <tuple>
#include <type_traits>

#include "../entities/entity.cc"
#include "../floor/cell.cc"
#include "../floor/position.cc"
#include "../floor/room.cc"
#include "factory-base.cc"
#include "floor-region.cc"
#else
import <memory>;
import <tuple>;
import <stdexcept>;
import <functional>;
import <type_traits>;
import <set>;
import cell;
import entity;
import factorybase;
import floorregion;
import position;
import room;
#endif  // __INTELLISENSE__

// A StandardFactory is a base class for Factories that create a single type of Entity.
//
// The StandardFactory is templated on the type of Entity it creates, and the types of any additional arguments that are required to create that Entity.
// Any methods marked with const are determinstic and safe to use without a preset strategy.
export template <typename T, typename... Args>
// Only allow T if it inherits from or is of type Entity.
  requires std::is_base_of_v<Entity, T>
class StandardFactory : public FactoryBase {
  // Create an Entity at the given position using random parameters.
  std::shared_ptr<T> create(const Position& position);

  // Return a tuple of randomly generated arguments to be used for creating an Entity.
  virtual std::tuple<Args...> generateArgs() = 0;

 public:
  // Deterministically create an Entity at the given position, using the given additional arguments.
  // Inteded to be used for creating specific entities with known parameters.
  //
  virtual std::shared_ptr<T> create(const Position& position, const Args... args) const = 0;

 protected:
  // Create an Entity at a randomly chosen tile within the given FloorRegion.
  virtual void processRoom(FloorRegion& region) override final;

  using FactoryBase::FactoryBase;
};

// Partial specialization for zero arguments (Staircase)

template <typename T>
  requires std::is_base_of_v<Entity, T>
class StandardFactory<T> : public FactoryBase {
 public:
  // Single create method - works for both random and deterministic paths.
  virtual std::shared_ptr<T> create(const Position& position) const = 0;

 protected:
  virtual void processRoom(FloorRegion& region) override final;
  using FactoryBase::FactoryBase;
};

#pragma region Implementation
template <typename T>
  requires std::is_base_of_v<Entity, T>
void StandardFactory<T>::processRoom(FloorRegion& region) {
  Position selectedPosition = selectRandomSpawn(region, rng);
  region.room[selectedPosition].add(create(selectedPosition));
}
template <typename T, typename... Args>
  requires std::is_base_of_v<Entity, T>
void StandardFactory<T, Args...>::processRoom(FloorRegion& region) {
  Position selectedPosition = selectRandomSpawn(region, rng);
  region.room[selectedPosition].add(create(selectedPosition));
}

template <typename T, typename... Args>
  requires std::is_base_of_v<Entity, T>
std::shared_ptr<T> StandardFactory<T, Args...>::create(const Position& position) {
  return std::apply(
      [this, &position](auto&&... args) {
        return this->create(position, std::forward<decltype(args)>(args)...);
      },
      generateArgs());
}

#pragma endregion  // Implementation
