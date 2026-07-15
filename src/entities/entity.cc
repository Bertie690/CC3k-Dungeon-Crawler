export module entity;

#pragma once

class Entity {
  // The (possibly-nonexistent) cell this Entity is currently occupying.
  Cell* cell;

 public:
  virtual ~Entity() = 0;
};
