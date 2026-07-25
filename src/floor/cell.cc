export module cell;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <type_traits>
#include <vector>

#include "../entities/entity.cc"
#include "../enums/tile-type.cc"
#include "position.cc"
#else
import position;
import entity;
import <vector>;
import <memory>;
import tiletype;
import <type_traits>;
#endif  // __INTELLISENSE__

// A Cell represents a single cell on the Floor grid.
// It owns all Entities it contains (sharing ownership with Game for players).
export class Cell {
  // The underlying backing storage for the entities in this Cell.
  // Uses shared pointers to allow for shared ownership between the Cell and the Game (for players).
  std::vector<std::shared_ptr<Entity>> entities;

 public:
  const Position position;
  const TileType tileType;

  Cell(const Position& position, const TileType tileType);
  ~Cell();

  // Return a read-only view of all entities in this Cell.
  const std::vector<std::shared_ptr<Entity>>& getEntities() const;

  bool isWalkable() const;
  bool isOccupied() const;

  void add(std::shared_ptr<Entity> entity);
  void remove(const Entity& entity);
};
