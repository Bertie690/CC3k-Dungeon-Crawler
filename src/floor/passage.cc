export module passage;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>

#include "../enums/room-type.cc"
#include "cell.cc"
#include "room.cc"
#else
import <vector>;
import roomtype;
import cell;
import room;
#endif  // __INTELLISENSE__

using namespace std;

export class Passage final : public Room {
 public:
  Passage(const vector<Cell*>& cells);
  RoomType type() const override;
};
