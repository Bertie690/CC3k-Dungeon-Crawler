module passage;

#ifdef __INTELLISENSE__
#include "passage.cc"
#endif  // __INTELLISENSE__

Passage::Passage(const vector<Cell*>& cells) : Room{cells} {}
RoomType Passage::type() const { return RoomType::Passage; }
