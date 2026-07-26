export module goldpile;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/gold-size.cc"
#include "../floor/position.cc"
#include "character.cc"
#include "item.cc"
#else
import character;
import goldsize;
import item;
import position;
#endif  // __INTELLISENSE__

// A pile of gold that can be picked up by the player.
export class GoldPile : public Item {
  virtual void applyEffect(Character& character) override final;

 public:
  const GoldSize size;

  GoldPile(const Position position, const GoldSize size) noexcept;
};
