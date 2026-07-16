export module cell;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
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
#endif  // __INTELLISENSE__

using namespace std;

// A Cell represents a single cell on the Floor grid.
// It owns all Entities it contains (sharing ownership with Game for players).
export class Cell {
  Position position;
  TileType tileType;
  // The underlying backing storage for the entities in this Cell.
  // Uses shared pointers to allow for shared ownership between the Cell and the Game (for players).
  vector<shared_ptr<Entity>> entities;

 public:
  Cell(Position position, TileType tileType);

  const Position& getPosition() const;
  TileType getTileType() const;
  const vector<shared_ptr<Entity>>& getEntities() const;

  bool isWalkable() const;
  bool isOccupied() const;

  void add(Entity& entity);
  void remove(Entity& entity);
};
