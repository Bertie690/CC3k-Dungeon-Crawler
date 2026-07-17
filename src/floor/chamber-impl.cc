module chamber;

#ifdef __INTELLISENSE__
#include "chamber.cc"
#endif  // __INTELLISENSE__

using namespace std;

RoomType Chamber::type() const { return RoomType::Chamber; }
