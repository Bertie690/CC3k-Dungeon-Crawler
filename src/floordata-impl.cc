module floordata;

#ifdef __INTELLISENSE__
#include <iostream>
#include <string>
#include <vector>

#include "floordata.cc"
#endif

using namespace std;

// [LD] Assumes valid input, can add invalid handling if we decide on framework

bool isTerrain(char c) {
  switch (c) {
    case '#':
    case '|':
    case '-':
    case '.':
    case '+':
    case ' ':
      return true;
    default:
      return false;
  }
}

bool isEnemy(char c) {
  switch (c) {
    case 'H':
    case 'W':
    case 'E':
    case 'O':
    case 'M':
    case 'D':
    case 'L':
      return true;
    default:
      return false;
  }
}

SpawnKind getSpawnKind(char c) {
  if ('0' <= c && c <= '5') return SpawnKind::Potion;
  if ('6' <= c && c <= '9') return SpawnKind::Gold;
  if (c == '\\') return SpawnKind::Staircase;
  if (isEnemy(c)) return SpawnKind::Enemy;
  return SpawnKind::Player;  // if (c == '@')
}

FloorData readFloor(const vector<string>& lines, int startLine) {
  FloorData floor;
  for (int row = 0; row < FloorData::HEIGHT; ++row) {
    const string& line = lines[row + startLine];
    floor.terrain.push_back(line);

    for (int col = 0; col < FloorData::WIDTH; ++col) {
      const char c = line[col];
      // Add spawns to FloorData
      if (isTerrain(c)) continue;
      floor.spawns.push_back(SpawnData{getSpawnKind(c), c, Position{row, col}});
      // Spawns are placed on top of '.' floor tiles.
      // LD: are there instances where they're not? in halls?
      floor.terrain[row][col] = '.';
    }
  }
  return floor;
}

vector<FloorData> readFloors(istream& in) {
  vector<string> lines;
  string line;

  while (getline(in, line)) {
    lines.push_back(line);
  }

  vector<FloorData> floors;
  const int totalFloors = lines.size() / FloorData::HEIGHT;
  for (int floorLevel = 0; floorLevel < totalFloors; ++floorLevel) {
    int startLine = floorLevel * FloorData::HEIGHT;
    floors.push_back(readFloor(lines, startLine));
  }

  return floors;
}
