module passage;

#ifdef __INTELLISENSE__
#include <memory>
#include <utility>
#include <vector>

#include "passage.cc"
#else
import <memory>;
import <utility>;
import <vector>;
#endif  // __INTELLISENSE__

Passage::Passage(std::vector<std::unique_ptr<Cell>> cells) : Room(std::move(cells)) {}
RoomType Passage::type() const { return RoomType::Passage; }
