export module potion;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/item-type.cc"
#include "../enums/overlap-result.cc"
#include "../enums/potion-type.cc"
#include "../floor/position.cc"
#include "character.cc"
#include "entity.cc"
#include "item.cc"
#else
import character;
import entity;
import item;
import itemtype;
import overlapresult;
import potiontype;
import position;
#endif

export class Potion final : public Item {
  // Potions are used from an adjacent cell; walking onto one must not consume it.
  virtual bool hasCollision() const override { return false; }
  virtual OverlapResult onOverlap(Entity&) override { return OverlapResult::Enter; }
  virtual bool onUse(Entity& usingEntity) override;

 public:
  const PotionType potionType;

  Potion(Position position, PotionType potionType) noexcept
      : Item{position, ItemType::Potion}, potionType{potionType} {}

  void use(Character& character) const;

 protected:
  virtual void applyEffect(Character& character) override { use(character); }
};
