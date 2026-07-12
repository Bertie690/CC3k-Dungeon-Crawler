export module floordata;

#ifdef __INTELLISENSE__
#include <iostream>
#include <string>
#include <vector>

#include "position.cc"
#else
import <iostream>;
import <string>;
import <vector>;
import position;
#endif  // __INTELLISENSE__

using namespace std;

export enum class SpawnKind { Player, Staircase, Potion, Gold, Enemy };

export struct SpawnData {
  SpawnKind kind;

  // character used in floor files e.g. @, H, 3
  char spawnCode;

  Position position;
};

export struct FloorData {
  static const int HEIGHT = 25;
  static const int WIDTH = 79;

  // terrain is the floor under spawns
  vector<string> terrain;
  vector<SpawnData> spawns;
};

export vector<FloorData> readFloors(istream& in);
