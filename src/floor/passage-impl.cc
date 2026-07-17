module passage;

#ifdef __INTELLISENSE__
#include "passage.cc"
#endif  // __INTELLISENSE__

RoomType Passage::type() const { return RoomType::Passage; }
