export module passage;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/room-type.cc"
#include "room.cc"
#else
import roomtype;
import room;
#endif  // __INTELLISENSE__

export class Passage final : public Room {
 public:
  using Room::Room;
  virtual RoomType type() const noexcept override { return RoomType::Passage; }
};
