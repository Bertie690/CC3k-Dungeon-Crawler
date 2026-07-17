export module passage;

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
import room;
import cell;
#endif  // __INTELLISENSE__

export class Passage final : public Room {
 public:
  Passage(std::vector<std::unique_ptr<Cell>> cells);
  virtual RoomType type() const override;
};
