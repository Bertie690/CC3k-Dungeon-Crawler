export module overlapresult;

#pragma once

// The result of an Entity attempting to enter a Cell containing another Entity
// As it concerns the Floor.
// Entity side effects should be handled in the onOverlap methods.
export enum class OverlapResult {
  Blocked, // Prevents the move
  Enter, // Allow sharing the cell
  Consumed, // Remove the existing entity
  FloorTransition,  // Transition to the next floor or end game
};
