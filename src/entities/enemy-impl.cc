module enemy;

#ifdef __INTELLISENSE__
#include <vector>

#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../enums/tile-type.cc"
#include "../floor/cell.cc"
#include "../floor/room.cc"
#include "enemy.cc"
#else
import racetype;
import <vector>;
import action;
import direction;
import tiletype;
import cell;
import floor;
import room;
#endif  // __INTELLISENSE__

using namespace std;

double Enemy::getAccuracy(Character& defender) const { return 0.5; }
