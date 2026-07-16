module passage;

#ifdef __INTELLISENSE__
#include "passage.cc"
#endif  // __INTELLISENSE__

using namespace std;

Passage::Passage(const vector<Cell*>& cells) : Room{cells} {}
RoomType Passage::type() const { return RoomType::Passage; }
