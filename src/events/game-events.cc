export module gameevents;

#pragma once

#ifdef __INTELLISENSE__
#include <string>

#include "../entities/entity.cc"
#include "../enums/action.cc"
#include "../enums/race-type.cc"
#include "../floor/floor.cc"
#else
import <string>;
import entity;
import racetype;
import action;
import floor;
#endif  // __INTELLISENSE__

export struct GameQuitEvent {
  bool showScoreboard;
};
export struct GameOverEvent {};
export struct FreezeEnemiesEvent {};

// Notify that Game created a new Floor.
export struct NewFloorEvent {
  Floor* newFloor;
};

export struct RaceSelectEvent {
  // The race that has been selected.
  const RaceType raceType;
};

export struct PlayerActionEvent {
  // The action that the player has taken.
  const Action action;
};
