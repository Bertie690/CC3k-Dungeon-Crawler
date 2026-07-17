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

export class Item : public Entity {
 public:
  // The type of this item.
  const ItemType type;
  // Construct a new Item with the given type.
  Item(Position position, ItemType type) noexcept;

  virtual ~Item() = 0;
};