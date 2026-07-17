export module gameevents;

#pragma once

#ifdef __INTELLISENSE__
#include <string>

#include "../entities/entity.cc"
#include "../enums/action.cc"
#include "../enums/race-type.cc"
#include "../floor/floor.cc"
#include "../floor/position.cc"
#else
import <string>;
import position;
import entity;
import racetype;
import action;
import floor;
#endif  // __INTELLISENSE__

export struct EntityDeathEvent {
  // The entity that has died.
  const Entity& entity;
};

export struct EntityMoveEvent {
  // The entity that has moved.
  const Entity& entity;
  // The position the entity had moved from.
  const Position from;
  // The position the entity has moved to.
  const Position& to = entity.position;
};

export struct GameQuitEvent {
  bool showScoreboard;
};

export struct NewFloorEvent {
  Floor* newFloor;
};

export struct RaceSelectEvent {
  // The race that has been selected.
  const RaceType raceType;
};

export struct PlayerActionEvent {
  // The action that the player has taken.
  const PlayerAction action;
};