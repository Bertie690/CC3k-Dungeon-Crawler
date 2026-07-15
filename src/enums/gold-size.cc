export module goldsize;

#pragma once

// Enum representing the different sizes of gold piles. \
// Each one is mapped to their numeric value for use with static_cast.
export enum class GoldSize {
  None = 0,
  Small = 1,
  Normal = 2,
  MerchantHoard = 4,
  DragonHoard = 6,
};