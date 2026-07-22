export module overlapresult;

#pragma once

// The result of an Entity attempting to enter a Cell containing another Entity
// as it concerns the Floor.
// Entity-side effects should be handled in the onOverlap methods.
export enum class OverlapResult {
  // The move should be prevented.
  Blocked,
  // The move should be allowed, and the existing entity should remain in the Cell.
  Enter,
  // The existing entity should be removed from the Cell, and the move should be allowed.
  Consumed,
  // A floor transition should be triggered for the player, generating a new floor and/or ending the game.
  FloorTransition,
};
