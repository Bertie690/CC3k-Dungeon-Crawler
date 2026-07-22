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

export class GoldPile : public Item {
  const GoldSize size;

  virtual void applyEffect(Character& character) override;

 public:
  GoldPile(Position position, GoldSize size) noexcept;
};
