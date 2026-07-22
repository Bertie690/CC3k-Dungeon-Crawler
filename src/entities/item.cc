export module item;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/item-type.cc"
#include "../enums/overlap-result.cc"
#include "../floor/position.cc"
#include "character.cc"
#include "entity.cc"
#else
import character;
import entity;
import overlapresult;
import position;
import itemtype;
#endif  // __INTELLISENSE__

// An Item represents an Entity that can be picked up by a Character.
export class Item : public ConcreteEntity {
  // Return if the given Character can pick up this Item.
  virtual bool canPickup(Character& character) const;
  // Apply Item's effect to the Character.
  virtual void applyEffect(Character& character) = 0;

  virtual OverlapResult onOverlap(Entity& movingEntity) override;

 public:
  // The type of this item.
  const ItemType type;

  // Construct a new Item with the given type.
  Item(Position position, ItemType type) noexcept : ConcreteEntity{position}, type(type) {}
  virtual ~Item() = 0;
};

Item::~Item() = default;