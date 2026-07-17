export module item;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/item-type.cc"
#include "../floor/position.cc"
#include "entity.cc"
#else
import entity;
import position;
import itemtype;
#endif  // __INTELLISENSE__

// An Item represents an Entity that can be picked up by a Character.
// TODO: Add functionality
export class Item : public Entity {
 public:
  // The type of this item.
  const ItemType type;

  // Construct a new Item with the given type.
  Item(Position position, ItemType type) noexcept : Entity{position}, type(type) {}
  virtual ~Item() = 0;
};

Item::~Item() = default;