export module direction;

#pragma once

// A Direction represents one of the 8 cardinal directions in 2D space.
export enum class Direction {
  North,
  East,
  South,
  West,
  NorthEast,
  SouthEast,
  SouthWest,
  NorthWest,
};

// An array containing all cardinal directions in clockwise order.
export const Direction allDirections[8] = {
    Direction::North,     Direction::East,      Direction::South,     Direction::West,
    Direction::NorthEast, Direction::SouthEast, Direction::SouthWest, Direction::NorthWest,
};
