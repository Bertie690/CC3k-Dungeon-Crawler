export module goldsize;

#pragma once

#ifdef __INTELLISENSE__
#include <variant>
#include <stdexcept>
#else
import <stdexcept>;
import <variant>;
#endif  // __INTELLISENSE__

// Enum representing the different sizes of gold piles.
// Each one is mapped to their numeric value for use with static_cast.
export enum class GoldSize : unsigned int {
  None = 0,
  Small = 1,
  Normal = 2,
  MerchantHoard = 4,
  DragonHoard = 6,
};

// Struct representing the gold dropped upon an entity's demise.
// Please use designated initializers to construct this struct.
export struct GoldDrop {
  // The amount of gold to add instantaneously to the killer's inventory.
  const unsigned int instantAmount = 0;

  // The number of gold piles to drop upon the entity's demise.
  // Must be set to 0 if and only if pileSize is set to GoldSize::None.
  const unsigned int pilesDropped = 0;
  // The size of each pile.
  // Should be set to None if and only if pilesDropped is 0.
  const GoldSize pileSize = GoldSize::None;

  // Add together 2 GoldDrops with compatible pile sizes.
  // Throws an exception on invalid addition.
  GoldDrop operator+(const GoldDrop& other) {
    if (this->pileSize != GoldSize::None && other.pileSize != GoldSize::None &&
        this->pileSize != other.pileSize) {
      throw std::invalid_argument(
          "Cannot combine two GoldDrops with non-None pile sizes. Please use one GoldDrop with "
          "the desired pile size.");
    }
    return GoldDrop{.instantAmount = this->instantAmount + other.instantAmount,
                    .pilesDropped = this->pilesDropped + other.pilesDropped,
                    .pileSize = this->pileSize == GoldSize::None ? other.pileSize : this->pileSize};
  }
};
