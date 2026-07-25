export module goldsize;

#pragma once

// Enum representing the different sizes of gold piles.
// Each one is mapped to their numeric value for use with static_cast.
export enum class GoldSize {
  None = 0,
  Small = 1,
  Normal = 2,
  MerchantHoard = 4,
  DragonHoard = 6,
};

struct InstantGoldDrop {
  const unsigned int amount;
};
struct NormalGoldDrop {
  // The number of gold piles to drop upon the entity's demise.
  const unsigned int pilesDropped;
  // The size of each pile.
  const GoldSize pileSize;
};
using GoldDrop = std::variant<InstantGoldDrop, NormalGoldDrop>;
