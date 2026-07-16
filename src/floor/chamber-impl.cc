module chamber;

#ifdef __INTELLISENSE__
#include "chamber.cc"
#endif  // __INTELLISENSE__

using namespace std;

Chamber::Chamber(const vector<Cell*>& cells) : Room{cells} {}
RoomType Chamber::type() const { return RoomType::Chamber; }
