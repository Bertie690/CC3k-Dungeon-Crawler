export module floordata;

#ifdef __INTELLISENSE__
#include <iostream>
#include <string>
#include <vector>
#else
import <iostream>;
import <string>;
import <vector>;
#endif

using namespace std;

// LD: should this be its own position module?
export struct Position {
  int row;
  int col;
};

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
