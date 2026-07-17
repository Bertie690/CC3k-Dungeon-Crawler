export module chamber;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../enums/room-type.cc"
#include "cell.cc"
#include "room.cc"
#else
import <memory>;
import <vector>;
import roomtype;
import cell;
import room;
#endif  // __INTELLISENSE__

export class Chamber final : public Room {
 public:
  Chamber(std::vector<std::unique_ptr<Cell>> cells);
  virtual RoomType type() const override;
};
